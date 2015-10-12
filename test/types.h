/*
 * =====================================================================================
 *
 *       Filename:  types.h
 *
 *    Description:  type
 *
 *        Version:  1.0
 *        Created:  10/12/2015 10:41:34 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __TYPES_H_
#define __TYPES_H_

#include <vector>
#include <string>

namespace MyNameSpace
{
	struct ClientInfo
	{
		std::string ip;
		int id;
		int type;
		unsigned short port;
	};
	using ClientInfoList = std::vector<ClientInfo>;
}

#endif
