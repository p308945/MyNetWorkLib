/*
 * =====================================================================================
 *
 *       Filename:  LoadBalance.h
 *
 *    Description:  load balance
 *
 *        Version:  1.0
 *        Created:  09/28/2015 03:33:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __CONNECTIONLOADBALANCE_H_
#define __CONNECTIONLOADBALANCE_H_

#include "../base/MyConstHash.h"
#include <sstream>
#include <iostream>

namespace MyNameSpace
{
	class MyConnectionLoadBalance
	{
		public:
			class ServerInfo
			{
				public:
					ServerInfo(const std::string & ip, short port): mIp(ip), mPort(port)
					{
						std::ostringstream os;
						os<<"ServerLoadBalance ip:"<<ip<<" port:"<<port;
						mTags = os.str();
					}
					std::string GetTags() const { return mTags;}
				private:
					std::string mIp;
					short mPort;
					std::string mTags;
			};
			bool addServer(const std::string & ip, short port);
			void removeServer(const std::string & ip, short port);
			std::shared_ptr<ServerInfo> SearchServer(const std::string & ip, short port);
			void SetHashFunc(HashFunction fun)
			{
				mConnHash.SetHashFunc(fun);
			}
		private:
			MyConstHash<ServerInfo> mConnHash;
	};
}

#endif
