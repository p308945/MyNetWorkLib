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
		enum class COMMAND_TYPE : char
		{
			INNER,
			OUTTER
		};
		struct BaseCommand
		{
			BaseCommand(uint32_t cmdId, COMMAND_TYPE type) : mCmdId(cmdId), mType(type)
			{
			}
			uint32_t mCmdId;
			COMMAND_TYPE mType;			//0 InnerMessage, 1 OutterMessage,也就是服务器内部之间的消息是0，服务器和客户端之间的消息是1
		};
	}
}

#endif
