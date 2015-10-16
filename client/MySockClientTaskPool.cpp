/*************************************************************************
    > File Name: MySockClientTaskPool.cpp
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Sat 08 Aug 2015 02:31:39 AM
 ************************************************************************/
#include "MySockClientTaskPool.h"
#include "../base/MyLock.h"
#include <queue>
#include <deque>
#include <set>
#include "errno.h"

namespace MyNameSpace
{
	class ClientTaskQueue
	{
		public:
			void addTask(MySockClientTask *task)
			{
					MyScopeLock scopeLock(mLock);
					mClientTaskQueue.push(task);				
			}

			virtual void add(MySockClientTask *task) = 0;

			void checkQueue()
			{
				{
					MyScopeLock scopeLock(mLock);
					while(!mClientTaskQueue.empty())
					{
						MySockClientTask *task = mClientTaskQueue.front();
						mClientTaskQueue.pop();
						mClientTaskQueueTmp.push(task);
					}
				}

				while(!mClientTaskQueueTmp.empty())
				{
					MySockClientTask *task = mClientTaskQueueTmp.front();
					add(task);
					mClientTaskQueueTmp.pop();
				}
			}
		private:
			MyLock mLock;
			std::queue<MySockClientTask*, std::deque<MySockClientTask*> > mClientTaskQueue;
			std::queue<MySockClientTask*, std::deque<MySockClientTask*> > mClientTaskQueueTmp;
	};

	class MyClientIoThread : public MyThread, public ClientTaskQueue
	{
		public:
		MyClientIoThread(MySockClientTaskPool *pool, bool j = true) : MyThread(j), mPool(pool)
		{
			epfd = -1;
		}
		~MyClientIoThread()
		{
			if (epfd > 0)
			{
				close(epfd);
			}
		}
		void run();
		void add(MySockClientTask *task)
		{
			epoll_event ev;
			ev.events = EPOLLIN|EPOLLOUT|EPOLLPRI|EPOLLERR;				//EPOLLOUT在此处设置可能有busy loop现象，但是如果不设置会导致后面频繁设置EPOLLOUT，性能未必高，加上run每次有等待时间，所以还是在此处加上
			ev.data.ptr = task;
			task->addEpollEvent(epfd, ev);
			taskSet.insert(task);
		}
		void remove(MySockClientTask *task)
		{
			epoll_event ev;
			ev.events = EPOLLIN|EPOLLOUT|EPOLLPRI|EPOLLERR;
			task->delEpollEvent(epfd, ev);
			taskSet.erase(task);
		}
		int getTaskCount()
		{
			return taskSet.size();
		}
		bool init()
		{
			if (!MyThread::init())
			{
				return false;
			}
			epfd = epoll_create(32);
			if (epfd < 0)
			{
				std::cerr<<"MyClientIoThread epoll create failed!"<<std::endl;
				return false;
			}
			epev.resize(32);
			return true;
		}
		private:
		MySockClientTaskPool *mPool;
		int epfd;
		std::vector<epoll_event> epev;
		std::set<MySockClientTask *> taskSet;
	};

	void MyClientIoThread::run()
	{
		std::set<MySockClientTask *> taskWillDel;
		while(!isFini())
		{
			checkQueue();
			int retCode = epoll_wait(epfd, &epev[0], taskSet.size()/*常数级*/, 50/*ms*/);
			for (int i = 0; i < retCode; ++i)
			{
				if (epev[i].events&EPOLLIN || epev[i].events&EPOLLPRI)
				{
					MySockClientTask *task = static_cast<MySockClientTask *>(epev[i].data.ptr);
					int len = task->rcvBuffer();
					if (len < 0)
					{
						taskWillDel.insert(task);
					}
					else if (len == 0)
					{
						taskWillDel.insert(task);
					}
					task->getMsg();
				}
				else if (epev[i].events&EPOLLOUT)
				{
					MySockClientTask *task = static_cast<MySockClientTask*>(epev[i].data.ptr);
					int len = task->syncSendBuf();
					if (len < 0)
					{
						taskWillDel.insert(task);
					}
				}
				else if (epev[i].events&EPOLLERR)
				{
					MySockClientTask *task = static_cast<MySockClientTask *>(epev[i].data.ptr);
					taskWillDel.insert(task);
				}
			}
			if (!taskWillDel.empty())
			{
				std::set<MySockClientTask *>::iterator iter = taskWillDel.begin();
				for (; iter != taskWillDel.end(); ++iter)
				{
					(*iter)->disConn();
					mPool->addRecycleThread(*iter);
				}
				taskWillDel.clear();
			}
			usleep(5000);
		}
	}

	class MyClientRecycleThread : public MyThread, public ClientTaskQueue
	{
		public:
		MyClientRecycleThread(MySockClientTaskPool *pool, bool j = true) : MyThread(j), mPool(pool)
		{
		}
		void run();
		void add(MySockClientTask *task)
		{
			taskSet.insert(task);
		}
		void remove(MySockClientTask *task)
		{
			taskSet.erase(task);
		}
		private:
		MySockClientTaskPool *mPool;
		std::set<MySockClientTask *> taskSet;
	};

	void MyClientRecycleThread::run()
	{
		while(!isFini())
		{
			checkQueue();
			if (!taskSet.empty())
			{
				std::set<MySockClientTask *> willdel;
				std::set<MySockClientTask *>::iterator iter = taskSet.begin();
				for (; iter != taskSet.end(); ++iter)
				{
					if ((*iter)->conn())
					{
						mPool->addIoThread(*iter);
						willdel.insert(*iter);
					}
//					MySockClientTaskManager::getInstance().removeTask(*iter);
//					delete *iter;
				}
				for (iter = willdel.begin(); iter != willdel.end(); ++iter)
				{
					taskSet.erase(*iter);
				}
//				taskSet.clear();
			}
			usleep(10*1000);
		}
	}

	bool MySockClientTaskPool::addTask(MySockClientTask * task)
	{
		mRecycleThread->addTask(task);
		return true;
	}

	void MySockClientTaskPool::addIoThread(MySockClientTask * task)
	{
		mIoThread->addTask(task);
	}

	bool MySockClientTaskPool::init()
	{
		mRecycleThread = new MyClientRecycleThread(this);
		if (NULL == mRecycleThread)
		{
			return false;
		}
		mRecycleThread->init();
		mRecycleThread->start();
		mIoThread = new MyClientIoThread(this);
		if (NULL == mIoThread)
		{
			mRecycleThread->terminate();
			mRecycleThread->join();
			delete mRecycleThread;
			mRecycleThread = NULL;
			return false;
		}
		mIoThread->init();
		mIoThread->start();
		return true;
	}

	void MySockClientTaskPool::fini()
	{
		mRecycleThread->terminate();
		mRecycleThread->join();
		mIoThread->terminate();
		mIoThread->join();
	}

	MySockClientTaskPool::~MySockClientTaskPool()
	{
		if (NULL != mRecycleThread)
		{
			delete mRecycleThread;
		}
		if (NULL != mIoThread)
		{
			delete mIoThread;
		}
	}
	
	void MySockClientTaskPool::addRecycleThread(MySockClientTask *task)
	{
		mRecycleThread->addTask(task);
	}
}
