/*
 * =====================================================================================
 *
 *       Filename:  XmlconfigParse.h
 *
 *    Description:  XmlConfigParse
 *
 *        Version:  1.0
 *        Created:  10/12/2015 10:52:39 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __XMLCONFIGPARSE_H_
#define __XMLCONFIGPARSE_H_

#include "types.h"

namespace MyNameSpace
{
	namespace XmlConfig
	{
		bool loadClientConfig(const char *configFile, ClientInfoList &clientInfoList);
	}
}
#endif
