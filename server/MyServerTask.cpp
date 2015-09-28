/*
 * =====================================================================================
 *
 *       Filename:  MyServerTask.cpp
 *
 *    Description:  server tcp connection
 *
 *        Version:  1.0
 *        Created:  09/28/2015 10:53:59 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "MyServerTask.h"

namespace MyNameSpace
{
	bool MyServerTask::cmdParse(const char *msg, int len)
	{
		std::cout<<"xxx"<<msg<<std::endl;
//		mDispatcher.dispatch(msg, len);
	}
}
