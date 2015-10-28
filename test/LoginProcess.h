/*
 * =====================================================================================
 *
 *       Filename:  LoginProcess.h
 *
 *    Description:  Login Process
 *
 *        Version:  1.0
 *        Created:  10/20/2015 10:46:40 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef __LOGINPROCESS_H_
#define __LOGINPROCESS_H_

#include "BaseCmd.h"

namespace MyNameSpace
{
	class LoginProcess
	{
		public:
			LoginProcess() {}
			~LoginProcess() {}
		public:
			bool ReqLogin(const Command::BaseCommand *cmd, uint32_t len, int taskId);
			bool testProtobuf(const Command::BaseCommand *cmd, uint32_t len, int taskId);

	};
#endif
}
