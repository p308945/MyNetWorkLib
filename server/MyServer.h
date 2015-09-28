/*************************************************************************
    > File Name: MyServer.h
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Sat 08 Aug 2015 04:49:19 PM
 ************************************************************************/
#ifndef _MY_SERVER_H
#define _MY_SERVER_H

#include "MyBaseServer.h"
#include "../base/MySockTaskPool.h"
#include "../base/CmdDispatcher.h"

namespace MyNameSpace
{
	class MyServer : public MyBaseServer
	{
		public:
			MyServer() : mTaskPool(4, 32), mUniqueId(0), mDispatcher("MyServer")
			{
			}
			virtual bool init(int port);
			virtual bool reload();
			virtual bool newTask(int sock);
			void fini();
		private:
			virtual void initCallBack();
		private:
			MySockTaskPool mTaskPool;
			uint32_t mUniqueId;
			Dispatcher mDispatcher;
	};
}

#endif
