/*************************************************************************
  > File Name: MySockClientTaskPool.h
  > Author: huangyun
  > Mail: 895175589@qq.com 
  > Created Time: Fri 07 Aug 2015 10:56:08 PM
 ************************************************************************/
#ifndef _MYSOCK_CLIENT_TASK_POOL_H
#define _MYSOCK_CLIENT_TASK_POOL_H

#include "../base/MyThread.h"
#include "MySockClientTask.h"
namespace MyNameSpace
{
	class MyClientRecycleThread;
	class MyClientIoThread;
	class MySockClientTaskPool
	{
		public:
			MySockClientTaskPool() 
			{

			}
			~MySockClientTaskPool();
			bool addTask(MySockClientTask * task);
			void addIoThread(MySockClientTask *task);
			void addRecycleThread(MySockClientTask *task);
			bool init();
			void fini();
		private:
			MyClientIoThread *mIoThread;
			MyClientRecycleThread *mRecycleThread;
	};
}
#endif
