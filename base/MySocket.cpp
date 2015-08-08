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
		mSendBuffer.writeBuffer(buf, len);		
	}

	int MySocket::syncSendBuf()
	{
		int len = mSendBuffer.getLen();
		char * tmp = mSendBuffer.getCurPtr();
		int l = ::send(mSock, tmp, len, 0);
		if (l < 0)
		{
			if (errno != EINTR)
			{
				return -1;
			}
			return 0;
		}
		if (l == 0)
		{
			return 0;
		}
		mSendBuffer.setDrift(l);
		return l;
	}
}
