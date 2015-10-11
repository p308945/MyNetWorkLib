/*
 * =====================================================================================
 *
 *       Filename:  LoadBalanceCmd.h
 *
 *    Description:  loadbalance command file
 *
 *        Version:  1.0
 *        Created:  09/28/2015 02:14:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __LOADBALANCECMD_H_
#define __LOADBALANCECMD_H_

#include "CmdNumber.h"
#include "BaseCmd.h"

namespace MyNameSpace
{
	namespace Command
	{
		struct ReqLoadBalance : BaseCommand(REQ_LOADBALANCE_CMD, COMMAND_TYPE::OUTTER)
		{

		};
		struct RtnLoadBalance : BaseCommand(RTN_LOADBALANCE_CMD, COMMAND_TYPE::INNER)
		{
			RtnLoadBalance()
			{
				memset(this, 0x0, sizeof(*this));
			}
			unsigned char mIp[32];	
			unsigned short mPort;
		};
	}
}

#endif
