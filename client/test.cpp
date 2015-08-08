/*************************************************************************
    > File Name: tese.cpp
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Sat 08 Aug 2015 11:01:42 PM
 ************************************************************************/

#include "MyClient.h"

int main()
{
	MyNameSpace::MyClient client;
	std::multimap<int, int> ipPortPair;
	ipPortPair.insert(std::pair<int, int>(INADDR_ANY, 50002));
	if (!client.init(ipPortPair))
	{
		std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): client init fail"<<std::endl;
		exit(1);
	}
	client.mainLoop();
}
