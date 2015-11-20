/*
 * =====================================================================================
 *
 *       Filename:  MyCsvDataDefine.h
 *
 *    Description:  Csv Data Define
 *
 *        Version:  1.0
 *        Created:  11/20/2015 03:45:15 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __MYCSVDATADEFINE_H__
#define __MYCSVDATADEFINE_H__

#include <vector>
#include <stdint.h>

namespace MyNameSpace
{
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE*)0)->MEMBER)

	class CsvConfigBase
	{
	};

	struct stTest111Data
	{
		stTest111Data() : userLevel(0), consume(0), bloodVolume(0), attackPower(0)
		{
			memberOffsetVec.push_back(offsetof(stTest111Data, userLevel));
			memberOffsetVec.push_back(offsetof(stTest111Data, consume));
			memberOffsetVec.push_back(offsetof(stTest111Data, bloodVolume));
			memberOffsetVec.push_back(offsetof(stTest111Data, attackPower));
			memberOffsetVec.push_back(offsetof(stTest111Data, descrption));
		}
		uint32_t userLevel;
		uint32_t consume;
		uint32_t bloodVolume;
		uint32_t attackPower;
		std::string descrption;
		std::vector<size_t> memberOffsetVec;
	};

	struct stTest111 : public CsvConfigBase
	{
		std::vector<stTest111Data> mData;
	};
}

#endif
