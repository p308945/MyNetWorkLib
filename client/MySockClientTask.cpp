/*************************************************************************
    > File Name: MySockClientTask.cpp
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Sat 08 Aug 2015 11:13:13 PM
 ************************************************************************/
#include "MySockClientTask.h"

namespace MyNameSpace
{
	bool MySockClientTask::getMsg()
	{
		bool hasMsg = false;
		while(true)
		{
			std::vector<char> msg;
			int len = mSock.readBuffer(msg);
			if (len == 0)
			{
				break;
			}
			hasMsg = true;
			pushMsg(len, &msg[0]);
		}
		return hasMsg;
	}

	int MySockClientTask::rcvBuffer()
	{
		return mSock.rcvBuffer();
	}
	int MySockClientTask::addEpollEvent(int epfd, epoll_event & ev)
	{
		return mSock.addEpollEvent(epfd, ev);
	}
	int MySockClientTask::delEpollEvent(int epfd, epoll_event & ev)
	{
		return mSock.delEpollEvent(epfd, ev);
	}

	int MySockClientTask::syncSendBuf()
	{
		return mSock.syncSendBuf();
	}

	int MySockClientTask::sendDataWithBuffer(const char *buf, uint32_t len)
	{
		return mSock.sendDataWithBuffer(buf, len);
	}

	int MySockClientTask::conn()
	{
		mSockfd = ::socket(AF_INET, SOCK_STREAM, 0);
		if (mSockfd < 0)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): socket fail"<<std::endl;
			return -1;
		}

		int len = 64 * 1024;
		int ret;
		ret = ::setsockopt(mSockfd, SOL_SOCKET, SO_SNDBUF, (const char *)&len, sizeof(int));
		if ( 0 != ret)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): set send buf fail"<<std::endl;
			return false;
		}
		ret = ::setsockopt(mSockfd, SOL_SOCKET, SO_RCVBUF, (const char *)&len, sizeof(int));
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

		ret = ::connect(mSockfd, (const sockaddr *)&addr, sizeof(addr));
		if (0 != ret)
		{
			std::cerr<<__FUNCTION__<<"("<<__LINE__<<") ip :"<<mIp<<"port: "<<mPort <<" connect fail"<<std::endl;
			return false;
		}
		mConn = true;
		mSock.setSocket(mSockfd);
		return true;
	}
}
