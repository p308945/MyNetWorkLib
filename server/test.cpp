/*************************************************************************
    > File Name: test.cpp
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Fri 07 Aug 2015 09:58:04 PM
 ************************************************************************/
#include "MyServer.h"
#include "../base/MySockTaskManager.h"
//#include "../base/MyHashFunction.h"
#include "../base/MyConstHash.h"

int main()
{
	MyNameSpace::MyServer server;
	if (!server.init(50002))
	{
		std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): server init fail"<<std::endl;
		exit(1);
	}
	/*
	std::cout<<MyNameSpace::SDBMHash("tg34tg")<<std::endl;
	std::cout<<MyNameSpace::SDBMHash("vrevg45")<<std::endl;
	std::cout<<MyNameSpace::SDBMHash("vervgre")<<std::endl;
	std::cout<<MyNameSpace::SDBMHash("56y756fd")<<std::endl;
	std::cout<<MyNameSpace::SDBMHash("t45t54")<<std::endl;
	*/
	server.mainLoop();
	server.fini();
	return 0;
}
