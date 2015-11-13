/*************************************************************************
    > File Name: MySockTaskPool.cpp
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Sat 08 Aug 2015 02:31:39 AM
 ************************************************************************/
#include "MySockTaskPool.h"
#include "MySockTask.h"
#include "MyLock.h"
#include <queue>
#include <deque>
#include <set>
#include "errno.h"
#include "MySockTaskManager.h"
#include <climits>

namespace MyNameSpace
{

	class TaskQueue
	{
		public:
			void addTask(MySockTask *task)
			{
					MyScopeLock scopeLock(mLock);
					mTaskQueue.push(task);				
			}

			virtual void add(MySockTask *task) = 0;

			void checkQueue()
			{
				{
					MyScopeLock scopeLock(mLock);
					while(!mTaskQueue.empty())
					{
						MySockTask *task = mTaskQueue.front();
						mTaskQueueTmp.push(task);
						mTaskQueue.pop();
					}
				}

				while(!mTaskQueueTmp.empty())
				{
					MySockTask *task = mTaskQueueTmp.front();
					add(task);
					mTaskQueueTmp.pop();
				}
			}
		private:
			MyLock mLock;
			std::queue<MySockTask*, std::deque<MySockTask*> > mTaskQueue;
			std::queue<MySockTask*, std::deque<MySockTask*> > mTaskQueueTmp;
	};

	class MyIoThread : public MyThread, public TaskQueue
	{
		public:
		MyIoThread(MySockTaskPool *pool, bool j = true) : MyThread(j), mPool(pool)
		{
			epfd = -1;
		}
		~MyIoThread()
		{
			if (epfd > 0)
			{
				close(epfd);
			}
		}
		void run();
		void add(MySockTask *task)
		{
			epoll_event ev;
			ev.events = EPOLLIN|EPOLLOUT|EPOLLPRI|EPOLLERR;				//EPOLLOUT在此处设置可能有busy loop现象，但是如果不设置会导致后面频繁设置EPOLLOUT，性能未必高，加上run每次有等待时间，所以还是在此处加上
			ev.data.ptr = task;
			task->addEpollEvent(epfd, ev);
			taskSet.insert(task);
		}
		void remove(MySockTask *task)
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
			MyThread::init();
			int maxCount = mPool->getMaxCoonPerIo();
			epfd = epoll_create(maxCount);
			if (epfd < 0)
			{
				std::cerr<<__FUNCTION__<<":"<<__LINE__<<" MyIoThread epoll create failed! "<< std::endl;
				return false;
			}
			epev.resize(maxCount);
			return true;
		}
		private:
		MySockTaskPool *mPool;
		int epfd;
		std::vector<epoll_event> epev;
		std::set<MySockTask *> taskSet;
	};

	void MyIoThread::run()
	{
		std::set<MySockTask *> taskWillDel;
		while(!isFini())
		{
			checkQueue();
			int retCode = epoll_wait(epfd, &epev[0], taskSet.size()/*常数级*/, 50/*ms*/);
			for (int i = 0; i < retCode; ++i)
			{
				if (epev[i].events&EPOLLIN || epev[i].events&EPOLLPRI)
				{
					MySockTask *task = static_cast<MySockTask*>(epev[i].data.ptr);
					int len = task->rcvBuffer();
					if (len < 0)
					{
						taskWillDel.insert(task);
					}
					else if (len == 0)
					{
						std::cout<<"client close"<<std::endl;
						taskWillDel.insert(task);
					}
					int getMsgRet = task->getMsg();
					if (getMsgRet < 0)
					{
						taskWillDel.insert(task);
					}
				}
				else if (epev[i].events&EPOLLOUT)
				{
					MySockTask *task = static_cast<MySockTask*>(epev[i].data.ptr);
					int len = task->syncSendBuf();
					if (len < 0)
					{
						taskWillDel.insert(task);
					}
				}
				else if (epev[i].events&EPOLLERR)
				{
					MySockTask *task = static_cast<MySockTask*>(epev[i].data.ptr);
					taskWillDel.insert(task);
				}
			}
			if (!taskWillDel.empty())
			{
				std::cout<<"delete client size:"<<taskWillDel.size()<<std::endl;
				std::set<MySockTask *>::iterator iter = taskWillDel.begin();
				for (; iter != taskWillDel.end(); ++iter)
				{
					mPool->addRecycleThread(*iter);
					remove(*iter);
				}
				taskWillDel.clear();
			}
			usleep(5000);
		}
	}

	class MyRecycleThread : public MyThread, public TaskQueue
	{
		public:
		MyRecycleThread(MySockTaskPool *pool, bool j = true) : MyThread(j), mPool(pool)
		{
		}
		void run();
		void add(MySockTask *task)
		{
			taskSet.insert(task);
		}
		void remove(MySockTask *task)
		{
			taskSet.erase(task);
		}
		private:
		MySockTaskPool *mPool;
		std::set<MySockTask *> taskSet;
	};

	void MyRecycleThread::run()
	{
		while(!isFini())
		{
			checkQueue();
			if (!taskSet.empty())
			{
				std::set<MySockTask *>::iterator iter = taskSet.begin();
				for (; iter != taskSet.end(); ++iter)
				{
					if (NULL != *iter)
					{
						MySockTaskManager::getInstance().removeTask(*iter);
						delete *iter;
					}
				}
				taskSet.clear();
			}
			usleep(10*1000);
		}
	}

	MySockTaskPool::MySockTaskPool(int ioCount, int maxConnPerIo):mIoCount(ioCount), mMaxConnPerIo(maxConnPerIo)
	{
	}

	bool MySockTaskPool::addTask(MySockTask * task)
	{
		int count = mIoThreadPool.getThreadCOunt();
		int minCount = INT_MAX;
		int index = -1;
		for (int i = 0; i < count; ++i)
		{
			MyIoThread * thread = dynamic_cast<MyIoThread*>(mIoThreadPool.getThreadByIndex(i));
			if (NULL != thread)
			{
				if (!thread->isStart())
				{
					if (!thread->start())
					{
						std::cerr<<__FUNCTION__<<":"<<__LINE__<<"thread start failed! "<< std::endl;
						continue;
					}
					thread->addTask(task);
					return true;
				}
				if ((minCount > thread->getTaskCount()) && (thread->getTaskCount() < mMaxConnPerIo))
				{
					index = i;
				}
/* 				if (thread->getTaskCount() < mMaxConnPerIo)
				{
					thread->addTask(task);
					return true;
				}
				else
				{
					std::cout<<"i: "<<i<<" mMaxConnPerIo: "<<mMaxConnPerIo<<" thread->getTaskCount(): "<<thread->getTaskCount()<<std::endl;
				}
				*/
			}
		}
		if (-1 == index)
		{
			std::cerr<<__FUNCTION__<<":"<<__LINE__<<"task add error! "<< std::endl;
			return false;
		}
		MyIoThread * thread = dynamic_cast<MyIoThread*>(mIoThreadPool.getThreadByIndex(index));
		if (NULL != thread)
		{
			thread->addTask(task);
			return true;
		}
		return false;
	}

	void MySockTaskPool::addRecycleThread(MySockTask * task)
	{
		mRecycleThread->addTask(task);
	}

	bool MySockTaskPool::init()
	{
		mRecycleThread = new MyRecycleThread(this);
		if (NULL == mRecycleThread)
		{
			return false;
		}
		for (int i = 0; i < mIoCount; ++i)
		{
			MyIoThread * thread = new MyIoThread(this);
			if (NULL == thread || !thread->init())
			{
				delete mRecycleThread;
				mRecycleThread = NULL;
				mIoThreadPool.delAllThread();
				return false;
			}
			mIoThreadPool.putThread(thread);
		}
		return true;
	}

	void MySockTaskPool::fini()
	{
		mIoThreadPool.joinAll();
		if (NULL != mRecycleThread)
		{
			mRecycleThread->terminate();
			mRecycleThread->join();
		}
	}

	MySockTaskPool::~MySockTaskPool()
	{
		mIoThreadPool.delAllThread();
		if (NULL != mRecycleThread)
		{
			delete mRecycleThread;
		}
	}
}
