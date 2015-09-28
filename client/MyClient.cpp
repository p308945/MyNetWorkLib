/*************************************************************************
    > File Name: MyClient.cpp
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Sun 09 Aug 2015 01:10:29 AM
 ************************************************************************/

#include "MyClient.h"
#include "MyClientTaskManager.h"
#include "MyClientTask.h"

namespace MyNameSpace
{
	bool MyClient::init(std::multimap<int, int> &ipPortPair)
	{
		if (!mTaskPool.init())
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): task pool init fail"<<std::endl;
		}
		for (std::multimap<int,int>::iterator it = ipPortPair.begin(); it != ipPortPair.end(); ++it)
		{
			++mUniqueId;
			MyClientTask *task = new MyClientTask(mUniqueId, it->first, it->second);
			if (NULL != task)
			{
				MyClientTaskManager::getInstance().addTask(task);
				mTaskPool.addTask(task);
			}
		}
		return true;
	}

	void MyClient::mainLoop()
	{
		while(!isFini())
		{
			MyClientTaskManager::getInstance().doProcessMsg();
//			MyClientTaskManager::getInstance().testSend();
			usleep(3 * 1000);
		}
	}
}
