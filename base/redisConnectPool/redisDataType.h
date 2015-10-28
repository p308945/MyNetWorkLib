/*
 * =====================================================================================
 *
 *       Filename:  redisDataType.h
 *
 *    Description:  redis result Data Type
 *
 *        Version:  1.0
 *        Created:  10/27/2015 02:50:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __REDISDATATYPE_H_
#define __REDISDATATYPE_H_

#include <vector>
#include <cstdint>

class MyRedisDataType
{
	MyRedisDataType()
	{
		public:
			MyRedisDataType()
			{
				memset(this, 0x0, sizeof(*this));
			}

			const char *getBin() const
			{
				return &mBin[0];
			}

			size_t getBinSize() const
			{
				return mBin.size();
			}

			void setBool(bool b) const
			{
				mBool = b;
			}

			bool getBool() const
			{
				return mBool;
			}

			const char *getStr() const
			{
				return mStr;
			}

			std::vector<char>	mBin;
			uint64_t 			mUi64t;
			int64_t	 			mI64t;
			const char 			*mStr;
			double 				mDouble;
			float 				mFloat;
			uint32_t 			mUi32t;
			int32_t 			mI32t;
			uint16_t 			mUi16t;
			int16_t 			mI16t;
			uint8_t 			mUi8t;
			int8_t 				mI8t;
			bool 				mBool;
	}
};

#endif
