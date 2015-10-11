/*************************************************************************
  > File Name: MyBaseServer.cpp
  > Author: huangyun
  > Mail: 895175589@qq.com 
  > Created Time: Sat 08 Aug 2015 02:41:36 PM
 ************************************************************************/
#include "MyBaseServer.h"
#include <signal.h>
#include <iostream>
#include <unistd.h>

namespace MyNameSpace
{
	namespace
	{
		void handlerHup(int sig)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): get hup signal"<<std::endl;
			MyBaseServer::Container_IT iter = MyBaseServer::mServerContainer.begin();
			for (; iter != MyBaseServer::mServerContainer.end(); ++iter)
			{
				(*iter)->reload();
			}
		}
		void handlerCtrlC(int sig)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): get ctrl c signal"<<std::endl;
			MyBaseServer::Container_IT iter = MyBaseServer::mServerContainer.begin();
			for (; iter != MyBaseServer::mServerContainer.end(); ++iter)
			{
				(*iter)->fini();
			}
		}
	}
	MyBaseServer::Container MyBaseServer::mServerContainer;

	bool MyBaseServer::reload()
	{
		return true;
	}

	void MyBaseServer::regInnerCallBack(uint32_t cmdId, CallBackFunT fun)
	{
		mInnerDispatcher.regCallback(cmdId, fun);
	}

	void MyBaseServer::regOutterCallBack(uint32_t cmdId, CallBackFunT fun)
	{
		mOutterDispatcher.regCallback(cmdId, fun);
	}

	bool MyBaseServer::init(int port)
	{
		mServerContainer.push_back(this);
		bool ret = mTcpServer.bindPort(port);
		if (!ret)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): bind fail"<<std::endl;
			return false;
		}
		struct sigaction sigact;
		sigact.sa_handler = handlerHup;
		sigaction(SIGHUP, &sigact, NULL);

		sigact.sa_handler = handlerCtrlC;
		sigaction(SIGINT, &sigact, NULL);

		signal(SIGPIPE,SIG_IGN);
		return ret;
	}
	int MyBaseServer::serverProcess()
	{
		int ret = mTcpServer.acceptCallBack();
		if (ret >= 0)
		{
			newTask(ret);
		}
		return ret;
	}
	void MyBaseServer::mainLoop()
	{
		while(!isFini())
		{
			serverProcess();
			usleep(3*1000);
		}
	}
}
