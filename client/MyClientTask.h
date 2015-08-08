/*************************************************************************
    > File Name: MyClientTask.h
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Sat 08 Aug 2015 11:20:49 PM
 ************************************************************************/
#ifndef _MY_CLIENT_TASK_H_
#define _MY_CLIENT_TASK_H_
#include "MySockClientTask.h"

namespace MyNameSpace
{
	class MyClientTask : public MySockClientTask
	{
		public:
			virtual bool cmdParse(const char *msg, int len);
			MyClientTask(int id, int ip, int port) : MySockClientTask(id, ip, port)
			{

			}
	};
}
#endif
