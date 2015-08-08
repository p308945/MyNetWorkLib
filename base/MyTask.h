/*************************************************************************
    > File Name: MyTask.h
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Sat 08 Aug 2015 05:31:44 PM
 ************************************************************************/
#ifndef _MY_TASK_H_
#define _MY_TASK_H_
#include "MySockTask.h"

namespace MyNameSpace
{
	class MyTask : public MySockTask
	{
		public:
			virtual bool cmdParse(const char *msg, int len);
			MyTask(int sock, int id) : MySockTask(sock, id)
			{

			}
	};
}
#endif
