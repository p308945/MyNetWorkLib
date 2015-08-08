/*************************************************************************
    > File Name: MyTcpServer.cpp
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Sat 08 Aug 2015 01:41:10 PM
 ************************************************************************/

#include "MyTcpServer.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <string.h>

namespace MyNameSpace
{

	MyTcpServer::~MyTcpServer()
	{
		if (mEpfd >= 0)
		{
			::close(mEpfd);
		}
		if (mSock >= 0)
		{
			::close(mSock);
		}
	}

	bool MyTcpServer::bindPort(int port)
	{
		mEpfd = epoll_create(1);
		if (mEpfd < 0)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): epoll create fail"<<std::endl;
			return false;
		}
		mSock = ::socket(AF_INET, SOCK_STREAM, 0);	
		if (mSock < 0)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): socket fail"<<std::endl;
			return false;
		}
		int ret = -1;
		int opt = 1;
		ret = ::setsockopt(mSock, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(int));
		if ( 0 != ret)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): set reuse addr fail"<<std::endl;
			return false;
		}

		ret = ::setsockopt(mSock, SOL_SOCKET, SO_KEEPALIVE, (const char *)&opt, sizeof(int));
		if ( 0 != ret)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): set keep alive fail"<<std::endl;
			return false;
		}

		int idle = 30;
		ret = ::setsockopt(mSock, SOL_TCP, TCP_KEEPIDLE, (const char *)&idle, sizeof(int));
		if ( 0 != ret)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): set keepalive idle fail"<<std::endl;
			return false;
		}

		int interval = 10;
		ret = ::setsockopt(mSock, SOL_TCP, TCP_KEEPINTVL, (const char *)&interval, sizeof(int));
		if ( 0 != ret)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): set keepalive interval fail"<<std::endl;
			return false;
		}

		int keepCount = 3;
		ret = ::setsockopt(mSock, SOL_TCP, TCP_KEEPCNT, (const char *)&keepCount, sizeof(int));
		if ( 0 != ret)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): set keepalive keep count fail"<<std::endl;
			return false;
		}

		int len = 64 * 1024;
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
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(port);
		ret = ::bind(mSock, (sockaddr *)&addr, sizeof(addr));
		if (0 != ret)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): bind(" <<port<<") fail:"<<errno<<std::endl;
			return false;
		}
		ret = ::listen(mSock, mListenCount);
		if (0 != ret)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): listen fail"<<std::endl;
			return false;
		}
		struct epoll_event ev;
		ev.events = EPOLLIN;
		ret = epoll_ctl(mEpfd, EPOLL_CTL_ADD, mSock, &ev);
		if (0 != ret)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): epoll add fail"<<std::endl;
			return false;
		}
		return true;
	}

	int MyTcpServer::acceptCallBack()
	{
		struct epoll_event ev;
		int ret = epoll_wait(mEpfd,&ev, 1, 10);
		if (ret > 0)
		{
			if (ev.events & EPOLLIN)
			{
				int sock = ::accept(mSock, NULL, 0);
				if (sock >= 0)
				{
					return sock;
				}
				else
				{
					return -1;
				}
			}
		}
		return -1;
	}

	bool MyTcpServer::setNonBlock(bool b)
	{
		int flags = ::fcntl(mSock, F_GETFL, 0);		
		if (b)
		{
			::fcntl(mSock, F_SETFL, flags|O_NONBLOCK);
		}
		else
		{
			::fcntl(mSock, F_SETFL, flags&~O_NONBLOCK);
		}
		return true;
	}
}
