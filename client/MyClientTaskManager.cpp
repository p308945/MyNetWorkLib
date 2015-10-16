/*************************************************************************
    > File Name: MyClientTaskManager.cpp
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Sun 09 Aug 2015 01:01:25 AM
 ************************************************************************/

#include "MyClientTaskManager.h"

namespace MyNameSpace
{
	void MyClientTaskManager::addTask(MySockClientTask *task)
	{
		MyScopeLock lock(mLock);
		mTasks[task->getId()] = task;
	}
	
	void MyClientTaskManager::removeTask(MySockClientTask *task)
	{
		MyScopeLock lock(mLock);
		mTasks.erase(task->getId());
	}

	bool MyClientTaskManager::doProcessMsg()
	{
		MyScopeLock lock(mLock);
		for (Container_IT it = mTasks.begin(); it != mTasks.end(); ++it)
		{
			it->second->processMsg();
		}
		return true;
	}

	MySockClientTask * MyClientTaskManager::getTaskById(int id)
	{
		Container_IT it;
		it = mTasks.find(id);
		if (mTasks.end() == it)
		{
			return NULL;
		}
		return it->second;
	}

	void MyClientTaskManager::testSend()
	{
		std::cout<<"test send"<<std::endl;
		const char *tmp = "hello world!";
		MyScopeLock lock(mLock);
		for (Container_IT it = mTasks.begin(); it != mTasks.end(); ++it)
		{
			if (it->second->isConn())
			{
				it->second->sendDataWithBuffer(tmp, strlen(tmp));
			}
		}
	}
}
