/*
 * =====================================================================================
 *
 *       Filename:  MyCsvParse.cpp
 *
 *    Description:  Csv Config Parse
 *
 *        Version:  1.0
 *        Created:  11/19/2015 05:28:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "MyCsvParse.h"
#include "tinyxml.h"
#include <sstream>
#include <fstream>
#include <vector>
#include <memory>
#include "MyCsvDataDefine.h"
#include <functional>

namespace MyNameSpace
{
	namespace
	{
		class FileUtil
		{
			public:
				FileUtil(const std::string & fileName) : mStream(fileName)
			{
			}

				~FileUtil()
				{
					if (mStream.is_open())
					{
						mStream.close();
					}
				}

				operator std::fstream &()
				{
					return mStream;
				}

				std::fstream & GetInnerStream()
				{
					return mStream;
				}

				bool is_open()
				{
					return mStream.is_open();
				}

				bool eof()
				{
					return mStream.eof();
				}

			private:
				std::fstream mStream;
		};

#define ParseHead(fileName)	\
		FileUtil is(fileName); \
		if (!is.is_open()) \
		{\
			std::cerr<<"file open error! FileName: "<<fileName<<std::endl;\
			return false;\
		}\
		std::string field;\
		std::string varName;\
		std::string dataType;\
		if (is.eof())\
		{\
			std::cerr<<"file empty! filename: "<<fileName<<std::endl;\
			return false;\
		}\
		std::getline(is.GetInnerStream(), field);\
		std::cout<<"field:"<<field<<std::endl;\
		if (is.eof())\
		{\
			std::cerr<<"only field! filename: "<<fileName<<std::endl;\
			return false;\
		}\
		std::getline(is.GetInnerStream(), varName);\
		varName.erase(varName.find_last_not_of("\r") + 1);\
		std::cout<<"varName:"<<varName<<std::endl;\
		if (is.eof())\
		{\
			std::cerr<<"only field and varName! filename: "<<fileName<<std::endl;\
			return false;\
		}\
		\
		std::getline(is.GetInnerStream(), dataType);\
		std::cout<<"dataType:"<<dataType<<std::endl;\
		dataType.erase(dataType.find_last_not_of("\r") + 1);\
		std::vector<std::string> varNameVec;\
		std::vector<std::string> dataTypeVec;\
		std::string::size_type last_found = 0;\
		std::string::size_type found = varName.find_first_of(",");\
		while(true)\
		{\
			if (found == std::string::npos)\
			{\
				varNameVec.push_back(std::string(varName, last_found, varName.size() - last_found));	\
				break;\
			}\
			else\
			{\
				varNameVec.push_back(std::string(varName, last_found, found - last_found));	\
			}\
			last_found = found + 1;\
			found = varName.find_first_of(",", last_found);\
		}\
		\
		last_found = 0;\
		found = dataType.find_first_of(",");\
		while(true)\
		{\
			if (found == std::string::npos)\
			{\
				std::cout<<"back"<<(int)dataType.back()<<std::endl;\
				dataTypeVec.push_back(std::string(dataType, last_found, dataType.size() - last_found));	\
				break;\
			}\
			else\
			{\
				dataTypeVec.push_back(std::string(dataType, last_found, found - last_found));	\
			}\
			last_found = found + 1;\
			found = dataType.find_first_of(",", last_found);\
		}\
		\
		for (auto iter : varNameVec)\
		{\
			std::cout<<"varName: "<<iter<<std::endl;\
		}	\
		for (auto iter : dataTypeVec)\
		{\
			std::cout<<"dataType: "<<iter<<std::endl;\
		}	\
		if (varNameVec.size() != dataTypeVec.size())\
		{\
			std::cerr<<"varNameVec.size(): "<<varNameVec.size()<<"dataTypeVec.size(): "<<dataTypeVec.size()<<std::endl;\
			return false;\
		}


#define ParseData(className, dataName)		\
			std::string line;\
			std::string value;\
			std::shared_ptr<className> p(new className());\
			while(getline(is.GetInnerStream(), line, '\n'))\
			{\
				dataName data;\
				int i = 0;\
				last_found = 0;\
				found = line.find_first_of(",");\
				while(true)\
				{\
					if (found == std::string::npos)\
					{\
						value.assign(std::string(line, last_found, line.size() - last_found));\
					}\
					else\
					{\
						value.assign(std::string(line, last_found, found - last_found));\
					}\
					/*\
					std::cerr<<dataTypeVec[i]<<std::endl;\
					std::cerr<<"------------------------"<<std::endl;\
					for (auto tmp : MyCsvParse::mDataTypeMap)\
					{\
						std::cerr<<tmp.first<<" size:"<<tmp.first.size()<<std::endl;\
					}\
					std::cerr<<"------------------------"<<std::endl;\
					*/\
					auto iter = MyCsvParse::mDataTypeMap.find(dataTypeVec[i]);\
					if (MyCsvParse::mDataTypeMap.end() == iter)\
					{\
						std::cerr<<"No support Type:"<<dataTypeVec[i]<<std::endl;\
						return false;\
					}\
					switch(iter->second)\
					{\
						case MyCsvParse::eINT32:\
							{\
								*((int32_t*)((char *)(&data) + data.memberOffsetVec[i])) = atoi(value.c_str());\
							}\
							break;\
						case MyCsvParse::eUINT32:\
							{\
								*((uint32_t*)((char *)(&data) + data.memberOffsetVec[i])) = strtoul(value.c_str(), 0, 0);\
							}\
							break;\
						case MyCsvParse::eFLOAT:\
							{\
								*((float*)((char *)(&data) + data.memberOffsetVec[i])) = atof(value.c_str());\
							}\
							break;\
						case MyCsvParse::eDOUBLE:\
							{\
								*((double*)((char *)(&data) + data.memberOffsetVec[i])) = atof(value.c_str());\
							}\
							break;\
						case MyCsvParse::eSTRING:\
							{\
								((std::string *)((char *)(&data) + data.memberOffsetVec[i]))->assign(value);\
							}\
							break;\
						default:\
							{\
								std::cerr<<"impossible run here Type not support!!!"<<std::endl;\
								return false;\
							}\
							break;\
					}\
					if (found == std::string::npos)\
					{\
						break;\
					}\
					last_found = found + 1;\
					found = line.find_first_of(",", last_found);\
					++i;\
				}\
				std::cout<<"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<std::endl;\
				std::cout<<data.userLevel<<std::endl;\
				std::cout<<data.consume<<std::endl;\
				std::cout<<data.bloodVolume<<std::endl;\
				std::cout<<data.attackPower<<std::endl;\
				std::cout<<data.descrption<<std::endl;\
				p->mData.push_back(data);\
				line.clear(); \
			}\
			MyCsvParse::mConfigMap[fileName] = p; \
			return true;

		bool ParseTest111(const std::string &fileName)
		{
			std::string fullFileName = "csv/" + fileName;
			ParseHead(fullFileName);
			ParseData(stTest111, stTest111Data);
		}
	}

	std::unordered_map<std::string /* className */, std::shared_ptr<CsvConfigBase>> MyCsvParse::mConfigMap;
	std::map<std::string /* className */, std::function<bool (const std::string&)>> MyCsvParse::mParseFunc;
	std::map<std::string, MyCsvParse::DATA_TYPE> MyCsvParse::mDataTypeMap;
	bool MyCsvParse::init()
	{
		mDataTypeMap.clear();
		mDataTypeMap.insert(std::make_pair("uint32", eUINT32));
		mDataTypeMap.insert(std::make_pair("int32", eINT32));
		mDataTypeMap.insert(std::make_pair("float", eFLOAT));
		mDataTypeMap.insert(std::make_pair("double", eDOUBLE));
		mDataTypeMap.insert(std::make_pair("string", eSTRING));


		mParseFunc.insert(std::make_pair("111.csv", std::bind(ParseTest111, std::placeholders::_1)));
		parseXml();

		ParseCsvFile();
		return true;
	}

	bool MyCsvParse::parseXml()
	{
		TiXmlDocument stDoc;
		stDoc.LoadFile("csv/csvConfig.xml");
		stDoc.LoadFile("111");
		TiXmlElement *pElement = NULL;
		TiXmlElement *pChildElm = NULL;
		TiXmlElement *pRoot = stDoc.FirstChildElement();
		if (NULL != pRoot)
		{
			pElement = pRoot->FirstChildElement("Item");
			if( pElement != NULL )
			{
				for( pChildElm = pElement->FirstChildElement(); pChildElm != NULL ; pChildElm = pChildElm->NextSiblingElement() )
				{
					std::string fileName = pChildElm->Attribute("fileName");
					//				std::string className = pChildElm->Attribute("className");
					mConfigMap.insert(std::make_pair(fileName, nullptr));
				}
			}
		}
		return true;
	}

	bool MyCsvParse::ParseCsvFile()
	{
		for (auto iter : mConfigMap)
		{
			auto it = mParseFunc.find(iter.first);
			if (mParseFunc.end() == it)
			{
				std::cerr<<"csv parse Func not found! FileName: "<<iter.first<<std::endl;
				return false;
			}
			if (!(it->second(it->first)))
			{
				std::cerr<<" csv parse Func return false! FileName: "<<it->first<<std::endl;
				return false;
			}
		}
		return true;
	}
}
