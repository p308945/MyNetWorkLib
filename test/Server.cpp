/*
 * =====================================================================================
 *
 *       Filename:  Server.cpp
 *
 *    Description:  Server
 *
 *        Version:  1.0
 *        Created:  10/11/2015 02:51:33 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "Server.h"
#include "MyClientTaskManager.h"
#include "MyClientTask.h"
#include "MySockTaskManager.h"
#include "MyServerMsgProcess.h"
#include "MyServerTask.h"
#include "MyThread.h"

namespace MyNameSpace
{
	namespace
	{
		class ServerMsgProcess : public MyThread, public MySingleton<ServerMsgProcess>
		{
			private:
				friend class MySingleton<ServerMsgProcess>;
				ServerMsgProcess()
				{

				}
				~ServerMsgProcess()
				{

				}
			public:
				void run()
				{
					while (!isFini())
					{
						MySockTaskManager::getInstance().doProcessMsg();
						MyClientTaskManager::getInstance().doProcessMsg();
						usleep(5*1000);	
					}
				}
		};
	}

	bool Server::init(unsigned short port)
	{
		if (!mServerTaskPool.init())
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): mServerTaskPool init fail"<<std::endl;
			return false;
		}
		if (!MyBaseServer::init(port))
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): MyBaseServer init fail"<<std::endl;
			return false;
		}
		if (!mClientTaskPool.init())
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): mClientTaskPool init fail"<<std::endl;
			return false;
			return false;
		}
 		if (!ServerMsgProcess::getInstance().start())
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): ServerMsgProcess start fail"<<std::endl;
			return false;
		}
		return true;

	}
	bool Server::reload()
	{

		//TODO reload config
		return true;
	}
	void Server::initCallBack()
	{
	}
	bool Server::newTask(int sock)
	{
		std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): new task"<<std::endl;
		MyServerTask *task = new MyServerTask(sock, mServerUniqueId);		
		if (NULL == task)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): new task fail"<<std::endl;
			return false;
		}
		MySockTaskManager::getInstance().addTask(task);
		if (!mServerTaskPool.addTask(task))
		{
			MySockTaskManager::getInstance().removeTask(task);
			delete task;
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): add task fail"<<std::endl;
			return false;
		}
		++mServerUniqueId;
		return true;
	}

	bool Server::newClient(const char* ip, unsigned short port)
	{
		MyClientTask *task = new MyClientTask(mClientUniqueId, inet_addr(ip), port, &mInnerDispatcher, &mOutterDispatcher);
		if (NULL != task)
		{
			++mClientUniqueId;
			MyClientTaskManager::getInstance().addTask(task);
			mClientTaskPool.addTask(task);
			return true;
		}
		else
		{
			return false;
		}
	}

	void Server::fini()
	{
		MyBaseServer::fini();
		mServerTaskPool.fini();
		mClientTaskPool.fini();
		ServerMsgProcess::getInstance().terminate();
	}
}
