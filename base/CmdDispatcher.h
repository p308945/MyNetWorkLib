/*
 * =====================================================================================
 *
 *       Filename:  CmdDispatcher.h
 *
 *    Description:  message dispatcher
 *
 *        Version:  1.0
 *        Created:  09/28/2015 01:44:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __CMDDISPATCHER_H_
#define __CMDDISPATCHER_H_

#include <functional>
#include <memory>
//#include <unordered_map>
#include <map>
#include <string>
#include <iostream>
#include "../proto/BaseCmd.h"

namespace MyNameSpace
{
	using CallBackFunT = std::function< bool(const Command::BaseCommand *, uint32_t, int)>;
	class Dispatcher
	{
		public:
			Dispatcher(const std::string & name) : mName(name){}
			~Dispatcher(){}
			const std::string & getName() const { return mName;}
			void regCallback(uint32_t cmdId, CallBackFunT fun)
			{
//				funTable.insert(std::make_pair<uint32_t, std::function<bool <Command::BaseCommand *, uint32_t> > >(cmdId, fun));
				funTable[cmdId] = fun;
			}

			bool dispatcher(const Command::BaseCommand *cmd, uint32_t cmdLen, int taskId)
			{
				CallBackFunT fun = funTable[cmd->mCmdId];
				if (fun)
				{
					return fun(cmd, cmdLen, taskId);
				}
				else
				{
					std::cerr<<"cmdId:"<<cmd->mCmdId<<"not regeister callback"<<" taskId: "<<taskId<<std::endl;
				}
				return false;
			}

		private:
//			std::unordered_map<uint32_t, std::function< bool(BaseCommand *, uint32_t) > > funTable;
			std::map<uint32_t, CallBackFunT> funTable;
			std::string mName;
	};
}

#endif
