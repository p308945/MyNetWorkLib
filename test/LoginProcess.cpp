/*
 * =====================================================================================
 *
 *       Filename:  LoginProcess.cpp
 *
 *    Description:  Login Process
 *
 *        Version:  1.0
 *        Created:  10/20/2015 11:38:24 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "LoginProcess.h"
#include "LoginProcessCmd.h"
#include "MySockTask.h"
#include "MySockTaskManager.h"
#include "LoginProcessCmd.h"
#include "protobuf/test.pb.h"

namespace MyNameSpace
{
	bool LoginProcess::ReqLogin(const Command::BaseCommand *cmd, uint32_t len, int taskId)
	{
		MySockTask* task = MySockTaskManager::getInstance().getTaskByIdWithOutLock(taskId);
		std::cout<<" taskId: "<<taskId<<"struct msgid: "<<cmd->mCmdId<<std::endl;
		if (NULL != task)
		{
			const Command::ReqLogin *rcv = static_cast<const Command::ReqLogin *>(cmd);
			std::cout<<"login account: "<<rcv->account<<std::endl;
			Command::RtnLogin rtn;
			rtn.res = 1;
			strncpy(rtn.account, rcv->account, sizeof(rtn.account));
			std::cout<<"return message! len:"<<sizeof(rtn)<<" account :"<<rtn.account<<std::endl;
			task->sendDataWithBuffer(reinterpret_cast<const char *>(&rtn), sizeof(rtn));
		}
		return true;
	}

	bool LoginProcess::testProtobuf(const Command::BaseCommand *cmd, uint32_t len, int taskId)
	{
		MySockTask* task = MySockTaskManager::getInstance().getTaskByIdWithOutLock(taskId);
		std::cout<<" taskId: "<<taskId<<"protobuf msgid: "<<cmd->mCmdId<<std::endl;
		if (NULL != task)
		{
			const char *pBuf = (const char *)cmd;
			pBuf += sizeof(Command::BaseCommand);
			MyProtoBuf::CMsgLoginReq msg;
			msg.ParseFromArray(pBuf, len - sizeof(Command::BaseCommand));
			std::cout<<"login "<<msg.account()<<std::endl;

			Command::ProtobufBaseCommand rtn(Command::OutterCommand::TEST_PROTO_BUF_RTN, Command::COMMAND_TYPE::OUTTER);

			MyProtoBuf::CMsgLoginRtn rtnBuf;
			rtnBuf.set_ret(1);
			rtnBuf.SerializeToArray(rtn.protobuf, Command::MAX_PROTOBUF_SIZE - sizeof(Command::BaseCommand));
			uint32_t protoSize = rtnBuf.ByteSize();
			task->sendDataWithBuffer(reinterpret_cast<const char *>(&rtn), sizeof(Command::BaseCommand) + protoSize);
		}
		return true;
	}

}
