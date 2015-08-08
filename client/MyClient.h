/*************************************************************************
    > File Name: MyClient.h
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Sat 08 Aug 2015 07:10:10 PM
 ************************************************************************/
#ifndef _MY_CLIENT_H_
#define _MY_CLIENT_H_

#include <map>
#include "MySockClientTaskPool.h"

namespace MyNameSpace
{
	class MyClient
	{
		public:
			MyClient() : mUniqueId(0), mComplete(false)
			{

			}
			~MyClient()
			{

			}
			bool init(std::multimap<int, int> &ipPortPair);
			void mainLoop();
			bool isFini()
			{
				return mComplete;
			}
		private:
			uint32_t mUniqueId;
			MySockClientTaskPool mTaskPool;
			bool mComplete;
	};
}

#endif
