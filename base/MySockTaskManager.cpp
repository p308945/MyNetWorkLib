/*************************************************************************
    > File Name: MySockTaskManager.cpp
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Sat 08 Aug 2015 07:43:35 PM
 ************************************************************************/

#include "MySockTaskManager.h"

namespace MyNameSpace
{
	void MySockTaskManager::addTask(MySockTask *task)
	{
		MyScopeLock lock(mLock);
		mTasks[task->getId()] = task;
	}
	
	void MySockTaskManager::removeTask(MySockTask *task)
	{
		MyScopeLock lock(mLock);
		mTasks.erase(task->getId());
	}

	bool MySockTaskManager::doProcessMsg()
	{
		MyScopeLock lock(mLock);
		for (Container_IT it = mTasks.begin(); it != mTasks.end(); ++it)
		{
			it->second->processMsg();
		}
	}
}
