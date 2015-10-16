/*
 * =====================================================================================
 *
 *       Filename:  XmlconfigParse.cpp
 *
 *    Description:  XmlConfigure
 *
 *        Version:  1.0
 *        Created:  10/11/2015 08:59:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */


#include "tinyxml.h"
#include "types.h"
#include "XmlconfigParse.h"
#include <iostream>

namespace MyNameSpace
{
	namespace XmlConfig
	{
		bool loadClientConfig(const char *configFile, ClientInfoList &clientInfoList)
		{
			TiXmlDocument stDoc;
			stDoc.LoadFile(configFile);
			TiXmlElement *pElement = NULL;
			TiXmlElement *pChildElm = NULL;
			TiXmlElement *pRoot = stDoc.FirstChildElement();

			// 读取需要连接的IP地址
			pElement = pRoot->FirstChildElement("EXTRANET");
			if( pElement != NULL )
			{
				for( pChildElm = pElement->FirstChildElement(); pChildElm != NULL ; pChildElm = pChildElm->NextSiblingElement() )
				{
					const char *szIPAddr = pChildElm->Attribute("IPADDRESS");
					const char *szId = pChildElm->Attribute("ID");
					const char *szPort = pChildElm->Attribute("PORT");
					const char *szType = pChildElm->Attribute("TYPE");
					if (NULL == szIPAddr || NULL == szId || NULL == szPort || NULL == szType)
					{
						std::cerr<<__FUNCTION__<<": "<<__LINE__<<" clientConfig file error"<<std::endl;
						exit(1);
					}
					ClientInfo info;
					info.ip = szIPAddr;
					info.id = atoi(szId);
					info.port = atoi(szPort);
					info.type = atoi(szType);
					clientInfoList.push_back(info);
				}
			}
			else
			{
				printf("get extranet connect ip and port from config file failed\n");
				exit(1);
			}
			return true;
		}
	}
}
