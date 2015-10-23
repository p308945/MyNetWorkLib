#include <queue>
#include <deque>
#include <stdlib.h>
#include <memory.h>

namespace MyNameSpace
{
	typedef std::pair<int, char *>MsgPair;
	typedef std::pair<volatile bool, MsgPair>QueuePair;
	template <int QUEUE_SIZE = 102400>
		class MessageQueue
		{
			public:
				MessageQueue() : readPos(0), writePos(0)
			{
			}
				bool pushMsg(int len, char *msg)
				{
					char *buf = NULL;
					buf = new char[len];
					if (buf != NULL)
					{
						memcpy((void *)buf, (void *)msg, len);
						//			string s;
						//			s.assign(buf, len);
						//			cout << "len:" << len << "msg:" << s << endl;
						processCache();
						if (!msgQueue[writePos].first)
						{
							msgQueue[writePos].second.first = len;
							msgQueue[writePos].second.second = buf;
							msgQueue[writePos].first = true;
//							writePos = ++writePos % QUEUE_SIZE;
							++writePos;
							writePos %= QUEUE_SIZE;
						}
						else
						{
							cacheQueue.push(std::make_pair(len, buf));
						}
						return true;
					}
					return false;
				}
				MsgPair *getMsg()
				{
					MsgPair *ret = NULL;
					if (msgQueue[readPos].first)
					{
						ret = &msgQueue[readPos].second;
					}
					else
					{
//						processCache();		maybe multithread
						if (msgQueue[readPos].first)
						{
							ret = &msgQueue[readPos].second;
						}
					}
					return ret;
				}
				void pop()
				{
					if (msgQueue[readPos].first)
					{
						delete [] msgQueue[readPos].second.second;
						msgQueue[readPos].first = false;
//						readPos = ++readPos % QUEUE_SIZE;
						++readPos;
						readPos %= QUEUE_SIZE;
					}
					else
					{
						processCache();
						if (msgQueue[readPos].first)
						{
							delete[] msgQueue[readPos].second.second;
							msgQueue[readPos].first = false;
//							readPos = ++readPos % QUEUE_SIZE;
							++readPos;
							readPos %= QUEUE_SIZE;
						}
					}
				}
				void processCache()
				{
					while (!cacheQueue.empty())
					{
						if (!msgQueue[writePos].first)
						{
							MsgPair tmp = cacheQueue.front();
							msgQueue[writePos].second = tmp;
							msgQueue[writePos].first = true;
//							writePos = ++writePos % QUEUE_SIZE;
							++writePos;
							writePos %= QUEUE_SIZE;
							cacheQueue.pop();
						}
						else
						{
							return;
						}
					}
				}
				void clear()
				{
					while (getMsg())
					{
						pop();
					}
				}
			private:
				unsigned int readPos;
				unsigned int writePos;
				QueuePair msgQueue[QUEUE_SIZE];
				std::queue<MsgPair, std::deque<MsgPair> > cacheQueue;
		};
}
