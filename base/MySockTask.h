/*************************************************************************
    > File Name: MySockTask.h
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Fri 07 Aug 2015 08:16:14 AM
 ************************************************************************/

#ifndef _MYSOCK_TASK_H_
#define _MYSOCK_TASK_H_
#include "MyQueue.h"
#include "MySocket.h"

namespace MyNameSpace
{
	class MySockTask : public MyQueue
	{
		public:
			MySockTask(int sock, int id);
			virtual ~MySockTask() {}
			int getMsg();
			int rcvBuffer();
			int sendDataWithBuffer(const char *buf, uint32_t len);
			int addEpollEvent(int epfd, epoll_event & ev);
			int delEpollEvent(int epfd, epoll_event & ev);
			int syncSendBuf();
			int getId()
			{
				return mId;
			}
		private:
			MySocket mSock;
			int mId;
	};
}

#endif
