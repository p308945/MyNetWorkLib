/*
 * =====================================================================================
 *
 *       Filename:  Server.h
 *
 *    Description:  Server
 *
 *        Version:  1.0
 *        Created:  10/11/2015 02:05:35 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _HY_SERVER_H
#define _HY_SERVER_H

#include "MyBaseServer.h"
#include "MySockTaskPool.h"
#include "CmdDispatcher.h"
#include "MySockClientTaskPool.h"
#include "types.h"
#include "LoginProcess.h"
#include "MyCsvParse.h"

namespace MyNameSpace
{

	class Server : public MyBaseServer
	{
		public:
			Server() : mServerTaskPool(4, 4096), mServerUniqueId(0)
			{
			}
			virtual bool init(unsigned short port);
			virtual bool reload();
			virtual bool newTask(int sock);	//connection from remote
			void fini();
			bool newClient(const char *ip, unsigned short port, int serverId, int serverType); //connect to remote
			virtual void initCallBack();
		private:
			MySockTaskPool mServerTaskPool;
			MySockClientTaskPool mClientTaskPool;
			uint32_t mServerUniqueId;
			uint32_t mClientUniqueId;
			ClientInfoList clientInfoList;
			LoginProcess loginProcess;
	};
}
#endif
