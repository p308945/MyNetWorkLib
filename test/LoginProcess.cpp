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

namespace MyNameSpace
{
	bool LoginProcess::ReqLogin(const Command::BaseCommand *cmd, uint32_t len, int taskId)
	{
		MySockTask* task = MySockTaskManager::getInstance().getTaskByIdWithOutLock(taskId);
		std::cout<<taskId<<std::endl;
		if (NULL != task)
		{
			Command::RtnLogin rtn;
			rtn.res = 1;
			std::cout<<"return message! len:"<<sizeof(rtn)<<std::endl;
			task->sendDataWithBuffer(reinterpret_cast<const char *>(&rtn), sizeof(rtn));
		}
		return true;
	}
}
