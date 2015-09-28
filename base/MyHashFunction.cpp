/*
 * =====================================================================================
 *
 *       Filename:  MyHashFunction.cpp
 *
 *    Description:  Common Hash Function
 *
 *        Version:  1.0
 *        Created:  09/28/2015 04:37:47 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "MyHashFunction.h"
#include <algorithm>

namespace MyNameSpace
{
	uint32_t SDBMHash(const std::string &str)
	{
		uint32_t hash = 0;
		for_each(str.begin(), str.end(), [&hash](char c){ hash = c + (hash<<6) + (hash<<16) - hash; });
		return (hash & 0x7FFFFFFF);
	}
	uint32_t RSHash(const std::string &str)
	{
		uint32_t b = 378551;
		uint32_t a = 63689;
		uint32_t hash = 0;
		for_each (str.begin(), str.end(), [&hash, &a, b](char c)
				{ 
					hash = hash * a + c; 
					a *= b;
				});
		return (hash & 0X7FFFFFF);
	}
	uint32_t JSHash(const std::string &str)
	{
		uint32_t hash = 1315423911;
		for_each (str.begin(), str.end(), [&hash](char c){ hash ^= ((hash<<5) + c + (hash>>2)); });
		return (hash & 0X7FFFFFF);
	}
	uint32_t PJWHash(const std::string &str)
	{
		uint32_t BitsInUnignedInt = (uint32_t)(sizeof(uint32_t) * 8);
		uint32_t ThreeQuarters = (uint32_t)((BitsInUnignedInt * 3) / 4);
		uint32_t OneEighth = (uint32_t)(BitsInUnignedInt  / 8);
		uint32_t HighBits = (uint32_t)(0xFFFFFFFF) << (BitsInUnignedInt - OneEighth);
		uint32_t hash = 0;
		uint32_t test = 0;
		for_each (str.begin(), str.end(), [&hash, &test, ThreeQuarters, HighBits, OneEighth, BitsInUnignedInt](char c)
				{ 
					hash = (hash << OneEighth) + c; 
					if ((test = hash & HighBits) != 0)
					{
						hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
					}
				});
		return hash & 0x7FFFFFFF;
	}
	uint32_t ELFHash(const std::string &str)
	{
		uint32_t hash = 0;
		uint32_t x = 0;
		for_each (str.begin(), str.end(), [&hash, &x](char c)
				{
					hash = (hash << 4) + c;
					if ((x = hash & 0xF000000L) != 0)
					{
						hash ^= (x >> 24);
						hash &= ~x;
					}
				});
		return (hash & 0x7FFFFFFF);
	}
	uint32_t BKDRHash(const std::string &str)
	{
		uint32_t seed = 131;
		uint32_t hash = 0;
		for_each(str.begin(), str.end(), [&hash, seed](char c){ hash = hash *seed + c;});
		return (hash & 0x7FFFFFFF);
	}
	uint32_t DJBHash(const std::string &str)
	{
		uint32_t hash = 5381;
		for_each (str.begin(), str.end(), [&hash](char c) { hash += (hash << 5) + c;});
		return (hash & 0x7FFFFFFF);
	}
	uint32_t APHash(const std::string &str)
	{
		uint32_t hash = 0;
		int i;
		for_each (str.begin(), str.end(), [&hash, &i](char c)
				{
					if ((i & 1) == 0)
					{
						hash ^= ((hash << 7) ^ c ^(hash >> 3));
					}
					else
					{
						hash ^= (~((hash << 11) ^ c ^ (hash >> 5)));
					}
				});
		return (hash & 0x7FFFFFFF);
	}

	uint32_t FNVHash(const std::string &str)
	{
		const int prime = 0X811C9DC5;
		uint32_t hash = 0;
		for_each(str.begin(), str.end(), [&hash](char c) { hash *= prime; hash ^= c;});
		return (hash & 0x7FFFFFFF);
	}

}
