/*
 * =====================================================================================
 *
 *       Filename:  LoginProcessCmd.h
 *
 *    Description:  Login Command
 *
 *        Version:  1.0
 *        Created:  10/20/2015 11:46:56 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __LOGINPROCESSCMD_H_
#define __LOGINPROCESSCMD_H_
#include "CmdNumber.h"
#include "BaseCmd.h"
#include <cstring>

namespace MyNameSpace
{
	namespace Command
	{
		const int MAX_ACCOUNT_LEN = 48;
		struct ReqLogin : BaseCommand
		{
			ReqLogin() : BaseCommand(OutterCommand::REQ_LOGIN_CMD, COMMAND_TYPE::OUTTER)
			{
				memset(account, 0x0, sizeof(account));
			}
			char account[MAX_ACCOUNT_LEN];
		}__attribute__ ((packed));

		struct RtnLogin : BaseCommand
		{
			RtnLogin() : BaseCommand(OutterCommand::RTN_LOGIN_CMD, COMMAND_TYPE::OUTTER)
			{
				res = 0;
				memset(account, 0x0, sizeof(account));
			}
			int res;
			char account[MAX_ACCOUNT_LEN];
		}__attribute__ ((packed));
	}
}
#endif
