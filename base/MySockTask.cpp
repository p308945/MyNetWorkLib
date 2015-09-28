/*************************************************************************
    > File Name: MySockTask.cpp
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Fri 07 Aug 2015 08:22:12 AM
 ************************************************************************/

#include "MySockTask.h"
#include "MySocket.h"
#include <sys/epoll.h>
#include <arpa/inet.h>

namespace MyNameSpace
{
	MySockTask::MySockTask(int sock, int id): mSock(sock), mId(id)
	{
	}

	bool MySockTask::getMsg()
	{
		bool hasMsg = false;
		while(true)
		{
			int len = 0;
			std::vector<char> msg;
			len = mSock.readBuffer(msg);
			if (len <= 0)
			{
				break;
			}
			hasMsg = true;
			std::string str(&msg[0], len);
//			std::cerr<<__FUNCTION__<<"("<<__LINE__<<") size :"<<len<<"str :"<<str<<std::endl;
			pushMsg(len, &msg[0]);
		}
		return hasMsg;
	}

	int MySockTask::rcvBuffer()
	{
		return mSock.rcvBuffer();
	}
	int MySockTask::addEpollEvent(int epfd, epoll_event & ev)
	{
		return mSock.addEpollEvent(epfd, ev);
	}
	int MySockTask::delEpollEvent(int epfd, epoll_event & ev)
	{
		return mSock.delEpollEvent(epfd, ev);
	}

	int MySockTask::syncSendBuf()
	{
		return mSock.syncSendBuf();
	}

	int MySockTask::sendDataWithBuffer(const char *buf, uint32_t len)
	{
		return mSock.sendDataWithBuffer(buf, len);
	}
}
