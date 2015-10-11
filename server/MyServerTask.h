/*
 * =====================================================================================
 *
 *       Filename:  MyServerTask.h
 *
 *    Description:  server tcp connection
 *
 *        Version:  1.0
 *        Created:  09/28/2015 10:51:10 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _MY_SERVER_TASK_H_
#define _MY_SERVER_TASK_H_

#include "../base/MyTask.h"
#include "../base/CmdDispatcher.h"

namespace MyNameSpace
{
	class MyServerTask : public MySockTask
	{
		public:
			virtual bool cmdParse(const char *msg, int len);
			MyServerTask(int sock, int id) : MySockTask(sock, id)
		{

		}
			MyServerTask(int sock, int id, Dispatcher *iDispatcher, Dispatcher *oDispatcher) : MySockTask(sock, id), mInnerDispatcher(iDispatcher), mOutterDispatcher(oDispatcher)
			{

			}
		private:
			Dispatcher *mInnerDispatcher;
			Dispatcher *mOutterDispatcher;
	};
}

#endif
