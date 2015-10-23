/*************************************************************************
  > File Name: MySocket.cpp
  > Author: huangyun
  > Mail: 895175589@qq.com 
  > Created Time: Thu 06 Aug 2015 11:54:45 PM
 ************************************************************************/

#include<iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include "MySocket.h"
#include "errno.h"

namespace MyNameSpace
{
	MySocket::MySocket(int sock) : mSock(sock)
	{
	}

	int MySocket::rcvBuffer()
	{
		char buf[64 * 1024];
		memset(buf, 0x0, sizeof(buf));
		int len = ::recv(mSock, buf, sizeof(buf), 0);
		if (len < 0)
		{
			if (errno != EINTR)
			{
				return -1;
			}
			return 0;
		}
		if (len == 0)
		{
			return 0;
		}
		mRecvBuffer.writeBuffer(buf, len);
		return len;
	}

	int MySocket::addEpollEvent(int epfd, epoll_event & ev)
	{
		return epoll_ctl(epfd, EPOLL_CTL_ADD, mSock, &ev);
	}

	int MySocket::delEpollEvent(int epfd, epoll_event & ev)
	{
		return epoll_ctl(epfd, EPOLL_CTL_DEL, mSock, &ev);
	}

	int MySocket::sendDataWithBuffer(const char *buf, uint32_t len)
	{
//		std::cerr<<__FUNCTION__<<"("<<__LINE__<<") size :"<<len<<"msg :"<<buf<<std::endl;
		return mSendBuffer.writeBuffer(buf, len);		
	}

	int MySocket::syncSendBuf()
	{
		/*
		int len = mSendBuffer.getLen();
		if (len == 0)
		{
			return 0;
		}
		char * tmp = mSendBuffer.getCurPtr();
		std::string str(tmp, len);
		std::cerr<<__FUNCTION__<<"("<<__LINE__<<") size :"<<str.size()<<"msg :"<<str<<std::endl;
		*/
		std::vector<char> msg;
		int len = mSendBuffer.readBuffer(msg);
		if (0 == len)
		{
			return 0;
		}
		int l = len;
		while(l > 0)
		{
//			std::cout<<"send buf:"<<l<<std::endl;
			int tmp = ::send(mSock, &msg[0], l, 0);
			if (tmp < 0)
			{
				if (errno != EINTR && errno != EAGAIN)
				{
					return -1;
				}
				continue;
			}
			l -= tmp;
		}
		return len;
	}

	int MySocket::readBuffer(std::vector<char> &msg)
	{
		return mRecvBuffer.readBuffer(msg);
	}

	uint16_t MySocket::getRemotePort()
	{
		sockaddr_in s;
		socklen_t len;
		len = sizeof(s);
		uint16_t port = 0;
		if ( 0 == getpeername(mSock, (sockaddr*)&s, &len))
		{
			port = ntohs(s.sin_port);
		}
		return port;
	}
	std::string MySocket::getRemoteIp()
	{
		sockaddr_in s;
		socklen_t len;
		std::string ip;
		len = sizeof(s);
		if ( 0 == getpeername(mSock, (sockaddr*)&s, &len))
		{
			ip= ::inet_ntoa(s.sin_addr);
		}
		return ip;
	}
}
