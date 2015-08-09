/*************************************************************************
    > File Name: MyTask.cpp
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Sat 08 Aug 2015 05:38:55 PM
 ************************************************************************/

#include "MyTask.h"

namespace MyNameSpace
{
	bool MyTask::cmdParse(const char *msg, int len)
	{
		std::cout<<"len: "<<len<<"msg: "<<msg<<std::endl;
		return true;
	}
}
