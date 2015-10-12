/*************************************************************************
    > File Name: MyClient.cpp
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Sat 08 Aug 2015 10:50:08 PM
 ************************************************************************/

#include <sys/socket.h>
#include "MyTcpClient.h"
#include <iostream>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>

namespace MyNameSpace
{
	int MyTcpClient::conn()
	{
		mSock = ::socket(AF_INET, SOCK_STREAM, 0);
		if (mSock < 0)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): socket fail"<<std::endl;
			return -1;
		}

		int len = 64 * 1024;
		int ret;
		ret = ::setsockopt(mSock, SOL_SOCKET, SO_SNDBUF, (const char *)&len, sizeof(int));
		if ( 0 != ret)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): set send buf fail"<<std::endl;
			return false;
		}
		ret = ::setsockopt(mSock, SOL_SOCKET, SO_RCVBUF, (const char *)&len, sizeof(int));
		if ( 0 != ret)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): set rcv buf fail"<<std::endl;
			return false;
		}	
		sockaddr_in addr;
		bzero(&addr, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = mIp;
		addr.sin_port = htons(mPort);

		ret = ::connect(mSock, (const sockaddr *)&addr, sizeof(addr));
		if (0 != ret)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<") ip :"<<mIp<<"port: "<<mPort <<" connect fail"<<std::endl;
			return false;
		}
		return true;
	}
}
