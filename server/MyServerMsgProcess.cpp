/*************************************************************************
    > File Name: MyServerMsgProcess.cpp
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Sat 08 Aug 2015 07:25:05 PM
 ************************************************************************/

#include "MyServerMsgProcess.h"
#include "../base/MySockTaskManager.h"

namespace MyNameSpace
{
	void MyServerMsgProcess::run()
	{
		while (!isFini())
		{
			MySockTaskManager::getInstance().doProcessMsg();
			usleep(5*1000);	
		}
	}
}
