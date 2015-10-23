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
//		mDispatcher.dispatch(msg, len);
		if (mInnerDispatcher && mOutterDispatcher)
		{
			const Command::BaseCommand *pCmd = reinterpret_cast<const Command::BaseCommand *>(msg);
//			std::cout<<"msg: "<<msg<<" Id:"<<pCmd->mCmdId<<" type: "<<(char)pCmd->mType<<" data: "<<pCmd->data<<" len: "<<len<<std::endl;
//			std::cout<<"msg: "<<msg<<" Id:"<<pCmd->mCmdId<<" type: "<<(char)pCmd->mType<<" len: "<<len<<std::endl;
			switch(pCmd->mType)
			{
				case Command::COMMAND_TYPE::INNER:
					{
						mInnerDispatcher->dispatcher(pCmd, len, getId());
					}
					break;
				case Command::COMMAND_TYPE::OUTTER:
					{
						mOutterDispatcher->dispatcher(pCmd, len, getId());
					}
					break;
				default:
					{
						std::cerr<<"file: "<<__FILE__<<" function: "<<__FUNCTION__<<" line: "<<__LINE__<<" Id: "<<pCmd->mCmdId<<" error"<<std::endl;
						return false;
					}
					break;
			}
		}
		return true;
	}
}
