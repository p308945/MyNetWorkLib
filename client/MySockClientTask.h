/*************************************************************************
  > File Name: MySockClientTask.h
  > Author: huangyun
  > Mail: 895175589@qq.com 
  > Created Time: Sat 08 Aug 2015 07:12:03 PM
 ************************************************************************/
#ifndef _MYSOCK_CLIENT_TASK_H_
#define _MYSOCK_CLIENT_TASK_H_
#include "../base/MyQueue.h"
#include "../base/MySocket.h"

namespace MyNameSpace
{
	class MySockClientTask : public MyQueue
	{
		public:
			MySockClientTask(int id, int ip, int port, int serverId, int serverType) :mId(id), mIp(ip), mPort(port), mSockfd(-1), mConn(false), mServerId(serverId), mServerType(serverType)
			{

			}
			bool getMsg();
			int rcvBuffer();
			int sendDataWithBuffer(const char *buf, uint32_t len);
			int addEpollEvent(int epfd, epoll_event & ev);
			int delEpollEvent(int epfd, epoll_event & ev);
			int syncSendBuf();
			int conn();
			int getId()
			{
				return mId;
			}
			bool isConn()
			{
				return mConn;
			}
			void disConn()
			{
				mConn = false;
			}
			int getType()
			{
				return mServerType;
			}
		private:
			MySocket mSock;
			int mId;
			int mIp;
			int mPort;
			int mSockfd;
			bool mConn;
			int mServerId;
			int mServerType;
	};
}
#endif
