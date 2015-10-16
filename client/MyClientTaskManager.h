/*************************************************************************
    > File Name: MyClientTaskManager.h
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Sun 09 Aug 2015 12:58:02 AM
 ************************************************************************/
#ifndef __MY_CLIENT_TASK_MANAGER_H_
#define __MY_CLIENT_TASK_MANAGER_H_

#include "../base/MySingleton.h"
#include "../base/MyLock.h"
#include "MySockClientTask.h"
#include <map>

namespace MyNameSpace
{
	class MyClientTaskManager : public MySingleton<MyClientTaskManager>
	{
		private:
			friend class MySingleton<MyClientTaskManager>;
			MyClientTaskManager()
			{

			}
			~MyClientTaskManager()
			{

			}
		public:
			void addTask(MySockClientTask *task);
			void removeTask(MySockClientTask *task);
			bool doProcessMsg();	
			void testSend();
			MySockClientTask *getTaskById(int);
		private:
			typedef std::map<int, MySockClientTask*> Container;
			typedef std::map<int, MySockClientTask*>::iterator Container_IT;
			Container mTasks;
			MyLock mLock;
	};
}
#endif
