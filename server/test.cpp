/*************************************************************************
    > File Name: test.cpp
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Fri 07 Aug 2015 09:58:04 PM
 ************************************************************************/
#include "MyServer.h"
#include "../base/MySockTaskManager.h"

int main()
{
	MyNameSpace::MyServer server;
	if (!server.init(50002))
	{
		std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): server init fail"<<std::endl;
		exit(1);
	}
	server.mainLoop();
	server.fini();
	return 0;
}
