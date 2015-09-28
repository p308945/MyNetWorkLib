/*
 * =====================================================================================
 *
 *       Filename:  MyHashFunction.h
 *
 *    Description:  Common Hash Function
 *
 *        Version:  1.0
 *        Created:  09/28/2015 04:28:08 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __MYHASHFUNCTION_H_
#define __MYHASHFUNCTION_H_

#include <stdint.h>
#include <string>
#include <functional>

namespace MyNameSpace
{
//	typedef uint32_t (*HashFunction)(const std::string &);
	typedef std::function<uint32_t (const std::string &)> HashFunction;
	uint32_t SDBMHash(const std::string &str);
	uint32_t RSHash(const std::string &str);
	uint32_t JSHash(const std::string &str);
	uint32_t PJWHash(const std::string &str);
	uint32_t ELFHash(const std::string &str);
	uint32_t BKDRHash(const std::string &str);
	uint32_t DJBHash(const std::string &str);
	uint32_t APHash(const std::string &str);
	uint32_t FNVHash(const std::string &str);
}

#endif
