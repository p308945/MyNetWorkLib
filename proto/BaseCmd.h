/*
 * =====================================================================================
 *
 *       Filename:  baseCmd.h
 *
 *    Description:  base command
 *
 *        Version:  1.0
 *        Created:  09/28/2015 02:25:50 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __BASECMD_H_
#define __BASECMD_H_

namespace MyNameSpace
{
	namespace Command
	{
		struct BaseCommand
		{
			BaseCommand(uint32_t cmdId) : mCmdId(cmdId)
			{
			}
			uint32_t mCmdId;
		};
	}
}

#endif
