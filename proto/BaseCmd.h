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

#include <cstdint>
#include <cstring>

//#pragma pack(1)

namespace MyNameSpace
{
	namespace Command
	{
		const uint32_t MAX_PROTOBUF_SIZE = 100 * 1024;
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
		}__attribute__ ((packed));

		struct ProtobufBaseCommand : BaseCommand
		{
			ProtobufBaseCommand(uint32_t cmdId, COMMAND_TYPE type) : BaseCommand(cmdId, type)
			{
				memset(protobuf, 0x0, sizeof(protobuf));
			}
			unsigned char protobuf[MAX_PROTOBUF_SIZE];
		}__attribute__ ((packed));
	}
}

#pragma pack()
#endif
