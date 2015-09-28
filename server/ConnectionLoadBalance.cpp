/*
 * =====================================================================================
 *
 *       Filename:  ConnectionLoadBalance.cpp
 *
 *    Description:  Connection Load Balance
 *
 *        Version:  1.0
 *        Created:  09/28/2015 09:36:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ConnectionLoadBalance.h"

namespace MyNameSpace
{
	bool MyConnectionLoadBalance::addServer(const std::string & ip, short port)
	{
		std::shared_ptr<ServerInfo> pInfo(new ServerInfo(ip, port));
		if (!pInfo)
		{
			std::cerr<<"addServer new shared ptr return NULL"<<"ip: "<<ip<<"port: "<<port<<std::endl;
		}
		return mConnHash.addNode(pInfo->GetTags(), MyConstHash<ServerInfo>::VIRTUAL_NODE_COUNT, pInfo);
	}

	void MyConnectionLoadBalance::removeServer(const std::string & ip, short port)
	{
		std::string tags = ServerInfo(ip, port).GetTags();
		return mConnHash.removeNode(tags);
	}

	std::shared_ptr<MyConnectionLoadBalance::ServerInfo> MyConnectionLoadBalance::SearchServer(const std::string & ip, short port)
	{
		std::string tags = ServerInfo(ip, port).GetTags();
		return mConnHash.searchNode(tags);
	}
}
