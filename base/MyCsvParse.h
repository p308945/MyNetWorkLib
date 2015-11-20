/*
 * =====================================================================================
 *
 *       Filename:  MyCsvParse.h
 *
 *    Description:  CSV file Parse
 *
 *        Version:  1.0
 *        Created:  11/19/2015 02:58:23 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __MYCSVPARSE_H__
#define __MYCSVPARSE_H__

#include <string>
#include <unordered_map>
#include "MySingleton.h"
#include "MyCsvDataDefine.h"
#include <memory>
#include <map>

namespace MyNameSpace
{
	class MyCsvParse : public MySingleton<MyCsvParse>
	{
		public:
			enum DATA_TYPE
			{
				eINT32,
				eUINT32,
				eFLOAT,
				eDOUBLE,
				eSTRING,

				eMax,
			};

		public:
			MyCsvParse()
			{
			}

			~MyCsvParse()
			{
			}

			static std::shared_ptr<CsvConfigBase> GetConfigByName(const std::string &className)
			{
				std::shared_ptr<CsvConfigBase> res;
				auto iter = mConfigMap.find(className);
				if (mConfigMap.end() != iter)
				{
					return res = iter->second;
				}
				return res;
			}
			bool init();

		private:
			bool parseXml();
			bool ParseCsvFile();
		private:
//			std::string mFileName;
//			std::string mSeparator;

		public:
			static std::map<std::string, DATA_TYPE> mDataTypeMap;
			static std::unordered_map<std::string /* className */, std::shared_ptr<CsvConfigBase>> mConfigMap;
			static std::map<std::string /* className */, std::function<bool (const std::string &)>> mParseFunc;
	};
}

#endif
