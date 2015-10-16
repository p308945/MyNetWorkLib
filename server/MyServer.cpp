/*************************************************************************
    > File Name: MyServer.cpp
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Sat 08 Aug 2015 04:56:34 PM
 ************************************************************************/

#include "MyServer.h"
#include "../base/MySockTaskManager.h"
#include "MyServerMsgProcess.h"
#include "MyServerTask.h"

namespace MyNameSpace
{
	bool MyServer::init(int port)
	{
		//TODO inti load config... and so on
		if (!mTaskPool.init())
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): MySockTaskPool init fail"<<std::endl;
			return false;
		}
		if (!MyBaseServer::init(port))
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): MyBaseServer init fail"<<std::endl;
			return false;
		}
		if (!MyServerMsgProcess::getInstance().start())
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): MyServerMsgProcess start fail"<<std::endl;
			return false;
		}
		return true;
	}

	bool MyServer::reload()
	{
		//TODO reload config
		return true;
	}
	void MyServer::initCallBack()
	{
	}
	bool MyServer::newTask(int sock)
	{
		std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): new task"<<std::endl;
		++mUniqueId;
		MyServerTask *task = new MyServerTask(sock, mUniqueId);		
		if (NULL == task)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): new task fail"<<std::endl;
			return false;
		}
		MySockTaskManager::getInstance().addTask(task);
		if (!mTaskPool.addTask(task))
		{
			MySockTaskManager::getInstance().removeTask(task);
			delete task;
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): add task fail"<<std::endl;
			return false;
		}
		return true;	
	}

	void MyServer::fini()
	{
		MyBaseServer::fini();
		mTaskPool.fini();
		MyServerMsgProcess::getInstance().terminate();
	}
}
