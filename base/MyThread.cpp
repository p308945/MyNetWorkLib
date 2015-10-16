/*************************************************************************
    > File Name: MyThread.cpp
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Fri 07 Aug 2015 12:25:55 AM
 ************************************************************************/

#include "MyThread.h"
#include <iostream>
#include <algorithm>

namespace MyNameSpace
{
	MyThread::MyThread(bool j) : pid(0), state(0), mJoinable(j), complete(false)
	{

	}
	void *MyThread::threadFun(void *arg)
	{
		MyThread *p = static_cast<MyThread *>(arg);
		p->setState(1);
		p->notifyStartSuccess();
		std::cerr<<__FUNCTION__<<":"<<__LINE__<<" thread "<< "start!"<<std::endl;
		p->run();
		return NULL;
	}
	bool MyThread::start()
	{
		if (isStart())
		{
			return true;
		}
		if (!init())
		{
			std::cerr<<__FUNCTION__<<":"<<__LINE__<<" MyThread init fail!"<<std::endl;
			return false;
		}
		int i = pthread_create(&pid, NULL, MyThread::threadFun, this);
		if (i != 0)
		{
			std::cerr<<"pthread create error"<< std::endl;
			return false;
		}
		mLock.lock();
		while(state != 1)
		{
			mCond.wait();
		}
		mLock.unlock();
		return true;
	}
	void MyThread::notifyStartSuccess()
	{
		mLock.lock();
		mCond.notify();
		mLock.unlock();
	}

	int MyThread::join()
	{
		return pthread_join(pid, NULL);
	}

	void MyThreadPool::putThread(MyThread* pThread)
	{
		mThreads.push_back(pThread);
	}
	void MyThreadPool::joinAll()
	{
		ContainerIt iter = mThreads.begin();
		for (; iter != mThreads.end(); ++iter)
		{
			(*iter)->terminate();
			if ((*iter)->isJoinAble())
			{
				(*iter)->join();
			}
		}
	}

	namespace
	{
		class DelFunctor
		{
			public:
				void operator()(MyThread *thread)
				{
					if (NULL != thread)
					{
						delete thread;
					}
				}
		};
		DelFunctor delfun;
	}

	void MyThreadPool::delAllThread()
	{
		std::for_each(mThreads.begin(), mThreads.end(), delfun);
		mThreads.clear();
	}
}
