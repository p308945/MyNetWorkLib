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
#include "../proto/BaseCmd.h"

namespace MyNameSpace
{
	bool MyServerTask::cmdParse(const char *msg, int len)
	{
		std::cout<<"xxx"<<msg<<std::endl;
//		mDispatcher.dispatch(msg, len);
		if (mInnerDispatcher && mOutterDispatcher)
		{
			const Command::BaseCommand *pCmd = reinterpret_cast<const Command::BaseCommand *>(msg);
			switch(pCmd->mType)
			{
				case Command::COMMAND_TYPE::INNER:
					{
						mInnerDispatcher->dispatcher(pCmd, len);
					}
					break;
				case Command::COMMAND_TYPE::OUTTER:
					{
						mOutterDispatcher->dispatcher(pCmd, len);
					}
					break;
				default:
					{
						std::cerr<<__FUNCTION__<<": "<<__LINE__<<"type "<<(int)pCmd->mType<<" error"<<std::endl;
						return false;
					}
					break;
			}
		}
		return true;
	}
}
