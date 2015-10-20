/*************************************************************************
    > File Name: MyClientTask.cpp
    > Author: huangyun
    > Mail: 895175589@qq.com 
    > Created Time: Sat 08 Aug 2015 11:22:40 PM
 ************************************************************************/

#include "MyClientTask.h"
#include "../proto/BaseCmd.h"

namespace MyNameSpace
{
	bool MyClientTask::cmdParse(const char *msg, int len)
	{
		std::cout<<"len: "<<len<<"msg: "<<msg<<std::endl;
		if (mInnerDispatcher && mOutterDispatcher)
		{
			const Command::BaseCommand *pCmd = reinterpret_cast<const Command::BaseCommand *>(msg);
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
						std::cerr<<__FUNCTION__<<": "<<__LINE__<<"type "<<(int)pCmd->mType<<" error"<<std::endl;
						return false;
					}
					break;
			}
		}
		return true;
	}
}
