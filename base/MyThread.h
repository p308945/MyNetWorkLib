/*************************************************************************
    > File Name: MyThread.h
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Fri 07 Aug 2015 12:11:57 AM
 ************************************************************************/

#include <vector>
#include <set>
#include "MyLock.h"
#include <sys/epoll.h>
#include <iostream>

#ifndef __MYTHREAD_H_
#define __MYTHREAD_H_
namespace MyNameSpace
{
	class MyThread
	{
		public:
			MyThread(bool j = false);
			virtual ~MyThread() {}
			bool start();
			static void* threadFun(void *);
			virtual void run() = 0;
			void notifyStartSuccess();
			pthread_t getId()
			{
				return pthread_self();
			}
			void setState(int s)
			{
				state = s;
			}
			bool isStart()
			{
				return state == 1;
			}
			void terminate()
			{
				complete = true;
			}
			bool isJoinAble()
			{
				return mJoinable;
			}
			int join();
			bool isFini()
			{
				return complete;
			}
			virtual bool init()
			{
				mCond.init(mLock.getMutex());
				return true;
			}
		private:
			pthread_t pid;
			volatile int state;		//0未开始运行 1正在运行 2死亡
			bool mJoinable;
			MyLock mLock;
			MyCond mCond;
			int epfd;
			std::vector<struct epoll_event> events;
			bool complete;
	};

	class MyThreadPool
	{
		public:
			void putThread(MyThread *pThread);
			MyThread * getThreadByIndex(int index)
			{
				return mThreads[index];
			}
			int getThreadCOunt()
			{
				return mThreads.size();
			}
			void joinAll();
			void delAllThread();
		private:
			typedef std::vector<MyThread *> Container;
			typedef std::vector<MyThread *>::iterator ContainerIt;
			Container mThreads;
	};
}
#endif
