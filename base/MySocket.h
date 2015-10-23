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
#include "MyLock.h"
#include <unistd.h>

namespace MyNameSpace
{

	const int BIGM = 32 * 1024;
	const int CHUNK = 32 * 1024;
	const int HEAD_LEN = sizeof(uint32_t);
	const int MAX_PACKET_LEN = 64 * 1024;
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
					int writeBuffer(const char *buf, uint32_t len)
					{
						MyScopeLock lock(mLock);
						std::string str(buf, len);
//						std::cerr<<__FUNCTION__<<"("<<__LINE__<<") len :"<<len<<"str:"<<str<<std::endl;
						uint32_t remainLen = mBuffer.size() - writePos;
						if (remainLen < len)
						{
							moveBuffer();
							uint32_t chunkSize = getChunkSize(mBuffer.size() + len - remainLen);
							std::cerr<<"chunkSize:"<<chunkSize<<std::endl;
							mBuffer.resize(chunkSize);
						}
						memcpy((void *)&mBuffer[writePos], (void *)buf, len);
						writePos += len;
						return len;
					}

					int readBuffer(std::vector<char> &msg)
					{
						MyScopeLock lock(mLock);
						int len = completePacketlen();
						if (len < 0)
						{
//							std::cerr<<__FUNCTION__<<"("<<__LINE__<<") no completePacket len :"<<len<<std::endl;
							return 0;
						}
						//包超出最大限制，数据出错，断开连接处理
						else if (len >= MAX_PACKET_LEN)
						{
							std::cerr<<__FUNCTION__<<"("<<__LINE__<<") packet size :"<<len<<" > "<<MAX_PACKET_LEN<<std::endl;
							return -1;
						}
						msg.resize(len);
						memcpy(&msg[0], &mBuffer[readPos + HEAD_LEN], len);
//						std::string str(&msg[0], len);
//						std::cerr<<__FUNCTION__<<"("<<__LINE__<<") len :"<<len<<"str:"<<str<<std::endl;
						readPos += (len + HEAD_LEN);
						return len;
					}
/*
					char *getPtrAndDrift(int & len)
					{
						int l = completePacketlen();
						std::cerr<<"l:"<<l<<std::endl;
						if (l < 0)
						{
							return NULL;
						}
						len = l;
						char * tmp = &mBuffer[readPos + HEAD_LEN];
						readPos += l + HEAD_LEN;
						return tmp;
					}
					*/

				private:
					void moveBuffer()
					{
						uint32_t len =  writePos - readPos;
						memmove(&mBuffer[0], &mBuffer[readPos], len);
						readPos = 0;
						writePos = len;
					}

					int completePacketlen()
					{
						if ((writePos - readPos) < HEAD_LEN)
						{
							return -1;
						}
						uint32_t len = *(uint32_t *)&mBuffer[readPos];
						if (len >= MAX_PACKET_LEN)
						{
							return MAX_PACKET_LEN;
						}
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
					MyLock mLock;
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
					int writeBuffer(const char *buf, uint32_t len)
					{
				//		std::cerr<<__FUNCTION__<<"("<<__LINE__<<") size :"<<len<<"msg :"<<buf<<std::endl;
						MyScopeLock lock(mLock);
						uint32_t remainLen = mBuffer.size() - writePos;
				//		std::cerr<<__FUNCTION__<<"("<<__LINE__<<") remain size :"<<remainLen<<std::endl;
						if (remainLen < len + HEAD_LEN)
						{
							moveBuffer();
							uint32_t chunkSize = getChunkSize(mBuffer.size() + len + HEAD_LEN - remainLen);
				//			std::cerr<<__FUNCTION__<<"("<<__LINE__<<") chunk size :"<<chunkSize<<std::endl;
							mBuffer.resize(chunkSize);
						}
						*(uint32_t *)&mBuffer[writePos] = len;
						memcpy((void *)&mBuffer[writePos + HEAD_LEN], (void *)buf, len);
						writePos += len + HEAD_LEN;
						return len + HEAD_LEN;
					}

					int readBuffer(std::vector<char> &msg)
					{
						MyScopeLock lock(mLock);
						int len = writePos - readPos;
						if (0 == len)
						{
							return 0;
						}
						msg.resize(len);
						memcpy(&msg[0], &mBuffer[readPos], len);
						readPos = writePos = 0;
						return len;
					}

					/*
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
					*/

				private:
					void moveBuffer()
					{
						uint32_t len =  writePos - readPos;
						memmove(&mBuffer[0], &mBuffer[readPos], len);
						readPos = 0;
						writePos = len;
					}
				private:
					std::vector<char> mBuffer;
					uint32_t readPos;
					uint32_t writePos;
					MyLock mLock;
			};
		public:
		MySocket(int sock = -1);
		~MySocket()
		{
			::close(mSock);
		}
		/*
		char *getPtrAndDrift(int & l)
		{
			return mRecvBuffer.getPtrAndDrift(l);
		}
		*/
		int rcvBuffer();
		int sendDataWithBuffer(const char *buf, uint32_t len);
		int addEpollEvent(int epfd, epoll_event & ev);
		int delEpollEvent(int epfd, epoll_event & ev);
		int syncSendBuf();
		uint16_t getRemotePort();
		std::string getRemoteIp();
		void setSocket(int sock)
		{
			mSock = sock;
		}
		int readBuffer(std::vector<char> &msg);
		private:
		int mSock;
		RecvBuffer<> mRecvBuffer;
		SendBuffer<> mSendBuffer;
		MyLock mLock;
	};
}
#endif
