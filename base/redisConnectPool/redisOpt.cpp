/*
 * =====================================================================================
 *
 *       Filename:  redisOpt.c
 *
 *    Description:  redis Operation
 *
 *        Version:  1.0
 *        Created:  10/26/2015 10:56:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "redisOpt.h"
#include <iostream>
#include <sstream>
#include <string.h>

namespace MyNameSpace
{
	MyRedisOpt::MyRedisOpt(const std::string &host, int port, int dbNum, const std::string &password) : mHost(host), mPort(port), mDbNum(dbNum), mPassword(password)
	{
		mContext = NULL;
		mReply = NULL;
	}

	MyRedisOpt::~MyRedisOpt()
	{
		this->fini();
	}

	bool MyRedisOpt::init()
	{
		struct timeval timeOut = {1, 500000};
		this->fini();
		mContext = redisConnectWithTimeout(mHost.c_str(), mPort, timeOut);
		if (mContext && mContext->err)
		{
			std::cerr<<"redis init failed:"<<mHost.c_str()<<","<<mPort<<"errno:"<<mContext->errstr<<std::endl;
			redisFree(mContext);
			mContext = NULL;
			return false;
		}
		else if (NULL == mContext)	
		{
			std::cerr<<"redis init failed:"<<mHost.c_str()<<","<<mPort<<std::endl;
			return false;
		}
		if (!mPassword.empty())
		{
			std::string authCmd = "AUTH " + mPassword;
			mReply = (redisReply*)redisCommand(mContext, authCmd.c_str());
			if (NULL == mReply)
			{
				std::cerr<<"redis init failed auth error:"<<mHost.c_str()<<","<<mPort<<","<<mPassword<<std::endl;
				//fini();
				return false;
			}
			else if(mReply->type == REDIS_REPLY_ERROR)
			{
				//fini();
				return false;
			}
			freeReplyObject(mReply);
			mReply = NULL;
			return true;
		}
		return true;
	}

	void MyRedisOpt::fini()
	{
		if (mReply)
		{
			freeReplyObject(mReply);
			mReply = NULL;
		}
		if (mContext)
		{
			redisFree(mContext);
			mContext = NULL;
		}
	}

	bool MyRedisOpt::getInt(const char *key, int64_t &value)
	{
		bool ret = false;
		if (NULL == mContext)
		{
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "GET %s", key);
		if (NULL == mReply)
		{
			//fini();
			return ret;
		}
		else
		{
			switch (mReply->type)
			{
				case REDIS_REPLY_INTEGER:
					{
						value = mReply->integer;
						ret = true;
					}
					break;
				case REDIS_REPLY_STRING:
					{
						value = ::strtoll(mReply->str, NULL, 10);
						ret = true;
					}
					break;
				default:
					{
						std::cerr<<"redis getInt failed:"<<"type: "<<mReply->type<<" type String:"<<getStrByType(mReply->type)<<std::endl;
					}
					break;
			}
			freeReplyObject(mReply);
			mReply = NULL;
		}
		return ret;
	}

	bool MyRedisOpt::setInt(const char *key, const int64_t &value)
	{
		bool ret = false;
		if (NULL == mContext)
		{
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "SET %s %lld", key, value);
		if (NULL == mReply)
		{
			std::cerr<<"redis set Int error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ERROR)
		{
			//fini();
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
			ret = true;
		}
		return ret;
	}

	bool MyRedisOpt::setExpire(const char *key, unsigned long long expireTime)
	{
		bool ret = false;
		if (0 == expireTime)
		{
			return ret;
		}
		if (NULL == mContext)
		{
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "EXPIRE %s %llu", key, expireTime);
		if (NULL == mReply)
		{
			std::cerr<<"redis set expire error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ERROR)
		{
			//fini();
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
			ret = true;
		}
		return ret;

	}

	bool MyRedisOpt::set(const char *key, const char* value)
	{
		bool ret = false;
		if (NULL == mContext)
		{
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "SET %s %s", key, value);
		if (NULL == mReply)
		{
			std::cerr<<"redis set error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ERROR)
		{
			//fini();
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
			ret = true;
		}
		return ret;
	}

	bool MyRedisOpt::setBin(const char *key, const char* value, int len)
	{
		bool ret = false;
		if (NULL == mContext)
		{
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "SET %s %b", key, value, len);
		if (NULL == mReply)
		{
			std::cerr<<"redis set bin error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ERROR)
		{
			//fini();
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
			ret = true;
		}
		return ret;
	}

	bool MyRedisOpt::getBin(const char *key, std::string &value)
	{
		bool ret = false;
		if (NULL == mContext)
		{
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "GET %s", key);
		if (NULL == mReply)
		{
			std::cerr<<"redis get Bin error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_STRING)
		{
			value.resize(mReply->len + 1);
			::memcpy(&(*value.begin()), mReply->str, mReply->len);
			freeReplyObject(mReply);
			mReply = NULL;
			ret = true;
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
		}
		return ret;

	}

	bool MyRedisOpt::setSet(const char *key, const char *value)
	{
		bool ret = false;
		if (NULL == mContext)
		{
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "SADD %s %s", key, value);
		if (NULL == mReply)
		{
			std::cerr<<"redis set Set error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ERROR)
		{
			//fini();
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
			ret = true;
		}
		return ret;
	}

	bool MyRedisOpt::setSet(const char *key, unsigned long value)
	{
		bool ret = false;
		if (NULL == mContext)
		{
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "SADD %s %lu", key, value);
		if (NULL == mReply)
		{
			std::cerr<<"redis set Set error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ERROR)
		{
			//fini();
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
			ret = true;
		}
		return ret;
	}

	bool MyRedisOpt::getSet(const char *key, std::set<std::string> &valueSet, unsigned int count)
	{
		bool ret = false;
		if (0 == count)
		{
			return ret;
		}
		if (NULL == mContext)
		{
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "SRANDMEMBER %s %u", key, count);
		if (NULL == mReply)
		{
			std::cerr<<"redis get Set error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ARRAY)
		{
			for (uint32_t i = 0; i < mReply->elements; ++i)
			{
				if (NULL != mReply->element[i]->str)
				{
					valueSet.insert(mReply->element[i]->str);
				}
			}
			ret = true;
			freeReplyObject(mReply);
			mReply = NULL;
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
		}
		return ret;
	}

	bool MyRedisOpt::getSet(const char *key, std::set<unsigned long> &valueSet, unsigned int count)
	{
		bool ret = false;
		if (0 == count)
		{
			return ret;
		}
		if (NULL == mContext)
		{
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "SRANDMEMBER %s %u", key, count);
		if (NULL == mReply)
		{
			std::cerr<<"redis get Set error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ARRAY)
		{
			for (uint32_t i = 0; i < mReply->elements; ++i)
			{
				if (NULL != mReply->element[i]->str)
				{
					valueSet.insert(strtoul(mReply->element[i]->str, NULL, 10));
				}
			}
			ret = true;
			freeReplyObject(mReply);
			mReply = NULL;
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
		}
		return ret;

	}

	bool MyRedisOpt::del(const char *key)
	{
		bool ret = false;
		if (NULL == mContext)
		{
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "DEL %s", key);
		if (NULL == mReply)
		{
			std::cerr<<"redis del error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ERROR)
		{
			//fini();
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
			ret = true;
		}
		return ret;
	}

	bool MyRedisOpt::delSet(const char *key, const char *value)
	{
		bool ret = false;
		if (NULL == mContext)
		{
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "SREM %s %s", key, value);
		if (NULL == mReply)
		{
			std::cerr<<"redis del Set error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ERROR)
		{
			//fini();
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
			ret = true;
		}
		return ret;

	}

	bool MyRedisOpt::delSet(const char *key, unsigned long value)
	{
		bool ret = false;
		if (NULL == mContext)
		{
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "DEL %s %lu", key, value);
		if (NULL == mReply)
		{
			std::cerr<<"redis del Set error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ERROR)
		{
			//fini();
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
			ret = true;
		}
		return ret;
	}

	MyRedisOpt::existRtn MyRedisOpt::isSet(const char *key, const char *value)
	{
		existRtn ret = existRtn::ENOTSETMEM;
		if (NULL == mContext)
		{
			ret = existRtn::EERROR;
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "SISMEMBER %s %s", key, value);
		if (NULL == mReply)
		{
			ret = existRtn::EERROR;
			std::cerr<<"redis is Set error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ERROR)
		{
			ret = existRtn::EERROR;
			//fini();
		}
		else
		{
			int i = 0;	
			switch (mReply->type)
			{
				case REDIS_REPLY_INTEGER:
					{
						i = mReply->integer;
					}
					break;
				case REDIS_REPLY_STRING:
					{
						i = ::atoi(mReply->str);
					}
					break;
				default:
					{
						std::cerr<<"redis is Set failed:"<<"type: "<<mReply->type<<" type String:"<<getStrByType(mReply->type)<<std::endl;
					}
					break;
			}
			freeReplyObject(mReply);
			mReply = NULL;
			if (0 != i)
			{
				ret = existRtn::ESETMEM;
			}
		}
		return ret;
	}

	MyRedisOpt::existRtn MyRedisOpt::isSet(const char *key, unsigned long value)
	{
		existRtn ret = existRtn::ENOTSETMEM;
		if (NULL == mContext)
		{
			ret = existRtn::EERROR;
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "SISMEMBER %s %lu", key, value);
		if (NULL == mReply)
		{
			ret = existRtn::EERROR;
			std::cerr<<"redis is Set error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ERROR)
		{
			ret = existRtn::EERROR;
			//fini();
		}
		else
		{
			int i = 0;	
			switch (mReply->type)
			{
				case REDIS_REPLY_INTEGER:
					{
						i = mReply->integer;
					}
					break;
				case REDIS_REPLY_STRING:
					{
						i = ::atoi(mReply->str);
					}
					break;
				default:
					{
						std::cerr<<"redis is Set failed:"<<"type: "<<mReply->type<<" type String:"<<getStrByType(mReply->type)<<std::endl;
					}
					break;
			}
			freeReplyObject(mReply);
			mReply = NULL;
			if (0 != i)
			{
				ret = existRtn::ESETMEM;
			}
		}
		return ret;
	}

	MyRedisOpt::existRtn MyRedisOpt::existKey(const char *key)
	{
		existRtn ret = existRtn::EKEYNOEXIST;
		if (NULL == mContext)
		{
			ret = existRtn::EERROR;
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "EXISTS %s ", key);
		if (NULL == mReply)
		{
			ret = existRtn::EERROR;
			std::cerr<<"redis exist Key error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ERROR)
		{
			ret = existRtn::EERROR;
			//fini();
		}
		else
		{
			int i = 0;
			switch (mReply->type)
			{
				case REDIS_REPLY_INTEGER:
					{
						i = mReply->integer;
					}
					break;
				case REDIS_REPLY_STRING:
					{
						i = ::atoi(mReply->str);
					}
					break;
				default:
					{
						std::cerr<<"redis is Set failed:"<<"type: "<<mReply->type<<" type String:"<<getStrByType(mReply->type)<<std::endl;
					}
					break;
			}
			freeReplyObject(mReply);
			mReply = NULL;
			if (0 != i)
			{
				ret = existRtn::EKEYEXIST;
			}
		}
		return ret;
	}

	bool MyRedisOpt::setZSet(const char *key, const unsigned long score, const char *value)
	{
		bool ret = false;
		if (NULL == mContext)
		{
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "ZADD %s %lu %s", key, score, value);
		if (NULL == mReply)
		{
			std::cerr<<"redis set ZSet error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ERROR)
		{
			//fini();
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
			ret = true;
		}
		return ret;
	}

	bool MyRedisOpt::delZSet(const char *key, const char *value)
	{
		bool ret = false;
		if (NULL == mContext)
		{
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "ZREM %s %s", key, value);
		if (NULL == mReply)
		{
			std::cerr<<"redis del ZSet error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ERROR)
		{
			//fini();
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
			ret = true;
		}
		return ret;
	}

	bool MyRedisOpt::addZSetScore(const char *key, unsigned long score, const char *value)
	{
		bool ret = false;
		if (NULL == mContext)
		{
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "ZINCRBY %s %lu %s", key, score, value);
		if (NULL == mReply)
		{
			std::cerr<<"redis add ZSet Score error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ERROR)
		{
			//fini();
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
			ret = true;
		}
		return ret;
	}

	bool MyRedisOpt::getZSetScore(const char *key, const char *value, unsigned long &score)
	{
		bool ret = false;
		if (NULL == mContext)
		{
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "ZSCORE %s %s", key, value);
		if (NULL == mReply)
		{
			std::cerr<<"redis get ZSet Score error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ERROR)
		{
			//fini();
		}
		else
		{
			switch (mReply->type)
			{
				case REDIS_REPLY_INTEGER:
					{
						score = mReply->integer;
						ret = true;
					}
					break;
				case REDIS_REPLY_STRING:
					{
						score = ::strtoul(mReply->str, NULL, 10);
						ret = true;
					}
					break;
				default:
					{
						std::cerr<<"redis is Set failed:"<<"type: "<<mReply->type<<" type String:"<<getStrByType(mReply->type)<<std::endl;
					}
					break;
			}
			freeReplyObject(mReply);
			mReply = NULL;
		}
		return ret;
	}

	bool MyRedisOpt::getZSet(const char *key, std::set<std::string> &valueSet, unsigned long start, unsigned long end)
	{
		bool ret = false;
		/*
		if (start > end) //一定不会相等，所以可以使用这个方法交换数据
		{
			start ^= end;
			end ^= start;
			start ^= end;
		}
		*/
		if (NULL == mContext)
		{
			return ret;
		}
		if (start < end)
		{
			mReply = (redisReply *)redisCommand(mContext, "ZRANGE %s %lu %lu", key, start, end);
		}
		else
		{
			mReply = (redisReply *)redisCommand(mContext, "ZRANGE %s %lu %lu", key, end, start);
		}
		if (NULL == mReply)
		{
			std::cerr<<"redis get ZSet error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ARRAY)
		{
			for (uint32_t i = 0; i < mReply->elements; ++i)
			{
				if (NULL != mReply->element[i]->str)
				{
					valueSet.insert(mReply->element[i]->str);
				}
			}
			ret = true;
			freeReplyObject(mReply);
			mReply = NULL;
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
		}
		return ret;
	}

	bool MyRedisOpt::getZSetWithScore(const char *key, std::map<std::string, unsigned long> &valueMap, const unsigned long start, const unsigned long end)
	{
		bool ret = false;
		/*
		if (start > end) //一定不会相等，所以可以使用这个方法交换数据
		{
			start ^= end;
			end ^= start;
			start ^= end;
		}
		*/
		if (NULL == mContext)
		{
			return ret;
		}
		if (start < end)
		{
			mReply = (redisReply *)redisCommand(mContext, "ZRANGE %s %lu %lu WITHSCORES", key, start, end);
		}
		else
		{
			mReply = (redisReply *)redisCommand(mContext, "ZRANGE %s %lu %lu WITHSCORES", key, end, start);
		}
		if (NULL == mReply)
		{
			std::cerr<<"redis get ZSet WithScore error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ARRAY)
		{
			if (mReply->elements > 0)
			{
				for (uint32_t i = 0; i < mReply->elements - 1; i += 2)
				{
					if (NULL != mReply->element[i + 1]->str)
					{
						std::pair<std::map<std::string, unsigned long>::iterator, bool> ret = valueMap.insert(std::make_pair(mReply->element[i]->str, ::strtoul(mReply->element[i + 1]->str, NULL, 10)));
						if (!(ret.second))
						{
							std::cerr<<"Warnning redis get ZSet WithScore insert error"<<std::endl;
						}
					}
				}
			}
			ret = true;
			freeReplyObject(mReply);
			mReply = NULL;
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
		}
		return ret;
	}

	bool MyRedisOpt::hSet(const char *key, const char *field, const char *value)
	{
		bool ret = false;
		if (NULL == mContext)
		{
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "HSET %s %s %s", key, field, value);
		if (NULL == mReply)
		{
			std::cerr<<"redis hSet error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ERROR)
		{
			//fini();
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
			ret = true;
		}
		return ret;

	}

	bool MyRedisOpt::hMSet(const char *key, std::map<std::string, std::string> fieldValue)
	{
		bool ret = false;
		if (NULL == mContext)
		{
			return ret;
		}
		std::ostringstream hMSetCmd;
		hMSetCmd<<"HMSET "<<key;
		for (auto iter : fieldValue)
		{
			hMSetCmd<<" "<<iter.first<<" "<<iter.second;
		}
		std::cout<<"HMSET "<<hMSetCmd.str().c_str()<<std::endl;
		mReply = (redisReply *)redisCommand(mContext, hMSetCmd.str().c_str());
		if (NULL == mReply)
		{
			std::cerr<<"redis  hMSet error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ERROR)
		{
			std::cerr<<"redis hMSet error REDIS_REPLY_ERROR"<<std::endl;
			//fini();
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
			ret = true;
		}
		return ret;
	}

	bool MyRedisOpt::hGet(const char *key, const char *field, std::string &value)
	{
		bool ret = false;
		if (NULL == mContext)
		{
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "HGET %s %s", key, field);
		if (NULL == mReply)
		{
			std::cerr<<"redis hGet error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_STRING)
		{
			value.resize(mReply->len + 1);
			::memcpy(&(*value.begin()), (const void *)(mReply->str), mReply->len);
			freeReplyObject(mReply);
			mReply = NULL;
			ret = true;
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
		}
		return ret;
	}

	bool MyRedisOpt::hMGet(const char *key, const std::set<std::string> &fieldSet, std::vector<std::string> &valueVec)
	{
		bool ret = false;
		if (NULL == mContext)
		{
			return ret;
		}
		std::ostringstream hMGetCmd;
		hMGetCmd<<"HMGET "<<key;
		for (auto iter : fieldSet)
		{
			hMGetCmd<<" "<<iter;
		}
		mReply = (redisReply *)redisCommand(mContext, hMGetCmd.str().c_str());
		if (NULL == mReply)
		{
			std::cerr<<"redis hMGet error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ARRAY)
		{
			for (uint32_t i = 0; i < mReply->elements; ++i)
			{
				if (NULL != mReply->element[i]->str)
				{
					valueVec.push_back(mReply->element[i]->str);
				}
			}
			freeReplyObject(mReply);
			mReply = NULL;
			ret = true;
		}
		else if (mReply->type == REDIS_REPLY_ERROR)
		{
			std::cerr<<"redis hMGet error REDIS_REPLY_ERROR"<<std::endl;
			//fini();
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
		}
		return ret;
	}

	bool MyRedisOpt::getKeys(std::set<std::string> &keySet, const char *keyFilter)
	{
		bool ret = false;
		if (NULL == mContext)
		{
			return ret;
		}
		mReply = (redisReply *)redisCommand(mContext, "KEYS %s", keyFilter);
		if (NULL == mReply)
		{
			std::cerr<<"redis getKeys error"<<std::endl;
			//fini();
		}
		else if(mReply->type == REDIS_REPLY_ARRAY)
		{
			for (uint32_t i = 0; i < mReply->elements; ++i)
			{
				if (NULL != mReply->element[i]->str)
				{
					keySet.insert(mReply->element[i]->str);
				}
			}
			freeReplyObject(mReply);
			mReply = NULL;
			ret = true;
		}
		else
		{
			freeReplyObject(mReply);
			mReply = NULL;
		}
		return ret;
	}

	const char *MyRedisOpt::getStrByType(int type)
	{
		switch (type)
		{
			case REDIS_REPLY_INTEGER:
				{
					return "REDIS_REPLY_INTEGER";
				}
				break;
			case REDIS_REPLY_STRING:
				{
					return "REDIS_REPLY_STRING";
				}
				break;
			case REDIS_REPLY_STATUS:
				{
					return "REDIS_REPLY_STRING";
				}
				break;
			case REDIS_REPLY_ERROR:
				{
					return "REDIS_REPLY_ERROR";
				}
				break;
			case REDIS_REPLY_NIL:
				{
					return "REDIS_REPLY_ERROR";
				}
				break;
			default:
				{
					return "UNKNOWN_TYPE";
				}
				break;
		}
	}

}
