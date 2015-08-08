/*************************************************************************
bool> File Name: MySocket.h
  > Author: huangyun
  > Mail: 895175589@qq.com 
  > Created Time: Thu 06 Aug 2015 11:22:23 PM
 ************************************************************************/
#ifndef _MY_SOCKET_H_
#define _MY_SOCKET_H_
#include <iostream>
#include <vector>
#include <string>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <memory.h>

namespace MyNameSpace
{

	const int BIGM = 64 * 1024;
	const int CHUNK = 64 * 1024;
	const int HEAD_LEN = sizeof(uint32_t);
	namespace
	{
		uint32_t getChunkSize(uint32_t size)
		{
			return (size + CHUNK - 1) / CHUNK * CHUNK;
		}
	}
	class MySocket
	{
		template<uint32_t SIZE = BIGM>
			class RecvBuffer
			{
				public:
					RecvBuffer()
					{
						mBuffer.resize(SIZE);
						readPos = writePos = 0;
					}
					void writeBuffer(const char *buf, uint32_t len)
					{
						moveBuffer();
						uint32_t remainLen = mBuffer.size() - writePos;
						if (remainLen < len)
						{
							uint32_t chunkSize = getChunkSize(mBuffer.size() + len - remainLen);
							mBuffer.resize(chunkSize);
						}
						*(uint32_t *)&mBuffer[writePos] = htonl(len);
						memcpy((void *)&mBuffer[writePos + HEAD_LEN], (void *)buf, len);
						writePos += len + HEAD_LEN;
					}

					char *getPtrAndDrift(int & len)
					{
						int l = completePacketlen();
						if (l < 0)
						{
							return NULL;
						}
						len = l;
						char * tmp = &mBuffer[readPos + HEAD_LEN];
						readPos += l + HEAD_LEN;
					}

				private:
					void moveBuffer()
					{
						uint32_t len =  writePos - readPos;
						memmove(&mBuffer[0], &mBuffer[readPos], len);
					}

					int completePacketlen()
					{
						if ((writePos - readPos) < 4)
						{
							return -1;
						}
						uint32_t len = ntohl(*(uint32_t *)&mBuffer[readPos]);
						if ((writePos - readPos) < len + HEAD_LEN)		//数据未收集满
						{
							return -1;
						}
						return len;
					}

				private:
					std::vector<char> mBuffer;
					uint32_t readPos;
					uint32_t writePos;
			};

		template<uint32_t SIZE = BIGM>
			class SendBuffer
			{
				public:
					SendBuffer()
					{
						mBuffer.resize(SIZE);
						readPos = writePos = 0;
					}
					void writeBuffer(const char *buf, uint32_t len)
					{
						moveBuffer();
						uint32_t remainLen = mBuffer.size() - writePos;
						if (remainLen < len)
						{
							uint32_t chunkSize = getChunkSize(mBuffer.size() + len - remainLen);
							mBuffer.resize(chunkSize);
						}
						memcpy((void *)&mBuffer[writePos], (void *)buf, len);
					}

					uint32_t getLen()
					{
						return writePos - readPos;	
					}

					char * getCurPtr()
					{
						return &mBuffer[readPos];
					}

					void setDrift(int d)
					{
						readPos += d;
					}

				private:
					void moveBuffer()
					{
						uint32_t len =  writePos - readPos;
						memmove(&mBuffer[0], &mBuffer[readPos], len);
						if (len > SIZE && mBuffer.capacity() > 2 * len)
						{
							mBuffer.resize(len);
						}
					}
				private:
					std::vector<char> mBuffer;
					uint32_t readPos;
					uint32_t writePos;
			};
		public:
		MySocket(int sock = -1);
		~MySocket()
		{
			close(mSock);
		}
		char *getPtrAndDrift(int & l)
		{
			return mRecvBuffer.getPtrAndDrift(l);
		}
		int rcvBuffer();
		int sendDataWithBuffer(const char *buf, uint32_t len);
		int addEpollEvent(int epfd, epoll_event & ev);
		int delEpollEvent(int epfd, epoll_event & ev);
		int syncSendBuf();
		private:
		int mSock;
		RecvBuffer<> mRecvBuffer;
		SendBuffer<> mSendBuffer;
	};
}
#endif
