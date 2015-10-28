/*
 * =====================================================================================
 *
 *       Filename:  redisOpt.h
 *
 *    Description:  redis opt
 *
 *        Version:  1.0
 *        Created:  10/26/2015 10:53:21 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __REDISOPT_H_
#define __REDISOPT_H_

#include <string>
#include "hiredis.h"
#include <cstdint>
#include <set>
#include <map>
#include <vector>

namespace MyNameSpace
{
	class MyRedisOpt
	{
		public:
			enum class existRtn : char
			{
				EERROR,
				EKEYEXIST,
				EKEYNOEXIST,

				ESETMEM,
				ENOTSETMEM,
			};
			MyRedisOpt(const std::string &host, int port, int dbNum, const std::string &password="");
			~MyRedisOpt();

			bool init();
			bool getInt(const char *key, int64_t &value);
			bool setInt(const char *key, const int64_t &value);
			bool setExpire(const char *key, unsigned long long expireTime);
			bool set(const char *key, const char* value);
			bool setBin(const char *key, const char* value, int len);
			bool getBin(const char *key, std::string &value);
			bool setSet(const char *key, const char *value);
			bool setSet(const char *key, unsigned long value);
			bool getSet(const char *key, std::set<std::string> &valueSet, unsigned int count);
			bool getSet(const char *key, std::set<unsigned long> &valueSet, unsigned int count);
			bool del(const char *key);
			bool delSet(const char *key, const char *value);
			bool delSet(const char *key, unsigned long value);
			existRtn isSet(const char *key, const char *value);
			existRtn isSet(const char *key, unsigned long value);
			existRtn existKey(const char *key);
			bool setZSet(const char *key, const unsigned long score, const char *value);
			bool delZSet(const char *key, const char *value);
			bool addZSetScore(const char *key, unsigned long score, const char *value); 
			bool getZSetScore(const char *key, const char *value, unsigned long &score);
			bool getZSet(const char *key, std::set<std::string> &valueSet, unsigned long start, unsigned long end);
			bool getZSetWithScore(const char *key, std::map<std::string, unsigned long> &valueMap, const unsigned long start, const unsigned long end);
			bool hSet(const char *key, const char *field, const char *value);
			bool hMSet(const char *key, std::map<std::string, std::string> fieldValue);
			bool hGet(const char *key, const char *field, std::string &value);
			bool hMGet(const char *key, const std::set<std::string> &fieldSet, std::vector<std::string> &valueVec);
			bool getKeys(std::set<std::string> &keySet, const char *keyFilter = "*");

		private:
			void fini();
			const char *getStrByType(int type);
		private:
			redisContext *mContext;
			redisReply *mReply;
			std::string mHost;
			int mPort;
			int mDbNum;
			std::string mPassword;
	};
}

#endif
