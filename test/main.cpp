/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  main
 *
 *        Version:  1.0
 *        Created:  10/11/2015 04:49:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "Server.h"

int main()
{
	MyNameSpace::Server s;
	if (!s.init(20000))
	{
		std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): server init fail"<<std::endl;
		exit(1);
	}
	s.mainLoop();
	s.fini();
	return 0;

}
