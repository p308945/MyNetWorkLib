/*
 * =====================================================================================
 *
 *       Filename:  test.cpp
 *
 *    Description:  test
 *
 *        Version:  1.0
 *        Created:  10/28/2015 11:27:51 AM
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

int main()
{
	MyNameSpace::MyRedisOpt redisConn("192.168.123.174", 6379, 0);
	redisConn.init();
	std::map<std::string, std::string> fieldValue;
	fieldValue.insert(std::make_pair("3", "3"));
	fieldValue.insert(std::make_pair("4", "4"));
	fieldValue.insert(std::make_pair("5", "5"));
	fieldValue.insert(std::make_pair("6", "6"));
	std::cout<<(int)redisConn.hMSet("test", fieldValue)<<std::endl;

	std::vector<std::string> value;
	std::set<std::string> field;
	field.insert("1");
	field.insert("2");
	field.insert("3");
	std::cout<<(int)redisConn.hMGet("test", field, value)<<std::endl;
	for (auto iter : value)
	{
		std::cout<<iter<<std::endl;
	}
	return 0;
}
