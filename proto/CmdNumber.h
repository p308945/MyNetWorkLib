/*
 * =====================================================================================
 *
 *       Filename:  LoadBalanceCmd.h
 *
 *    Description:  command Number file
 *
 *        Version:  1.0
 *        Created:  09/28/2015 11:42:07 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __CMDNUMBER_H_
#define __CMDNUMBER_H_

#include <stdint.h>

namespace MyNameSpace
{
	namespace Command
	{
		namespace OutterCommand
		{
			const uint32_t REQ_LOGIN_CMD = 1;
			const uint32_t RTN_LOGIN_CMD = 2;
			const uint32_t TEST_PROTO_BUF = 3;
			const uint32_t TEST_PROTO_BUF_RTN = 4;
		}

		namespace InnerCommand
		{
			const uint32_t REQ_LOADBALANCE_CMD = 1;
			const uint32_t RTN_LOADBALANCE_CMD = 2;
		}
	}
}

#endif
