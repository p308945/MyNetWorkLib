/*************************************************************************
    > File Name: MySockTaskManager.h
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Sat 08 Aug 2015 07:31:14 PM
 ************************************************************************/
#ifndef _MY_SOCK_TASK_MANAGER_H_
#define _MY_SOCK_TASK_MANAGER_H_

#include "MyTask.h"
#include "MySingleton.h"
#include <map>
#include "MyLock.h"

namespace MyNameSpace
{
	class MySockTaskManager : public MySingleton<MySockTaskManager>
	{
		private:
			friend class MySingleton<MySockTaskManager>;
			MySockTaskManager()
			{

			}
			~MySockTaskManager()
			{

			}
		public:
			void addTask(MySockTask *task);
			void removeTask(MySockTask *task);
			bool doProcessMsg();	
			MySockTask* getTaskByIdWithOutLock(int id);
			MySockTask* getTaskByIdWithLock(int id);
		private:
			typedef std::map<int, MySockTask*> Container;
			typedef std::map<int, MySockTask*>::iterator Container_IT;
			Container mTasks;
			MyLock mLock;		//暂时先共用一把锁，以后优化
	};
}

#endif
