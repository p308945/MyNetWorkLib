/*
 * =====================================================================================
 *
 *       Filename:  DBDefine.h
 *
 *    Description:  define database operation struct
 *
 *        Version:  1.0
 *        Created:  10/09/2015 10:26:34 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __DBDEFINE_H__
#define __DBDEFINE_H__

#include "DBPool.h"
#include <type_traits>

using namespace MyNameSpace;

/*example
const dbCol test_db_define[] = 
{
	{"USERID",	DB_DATA_TYPE::DB_ULONG,		8, NULL},
	{"MYNAME",	DB_DATA_TYPE::DB_STR,  		32, NULL},
	{"ISOK",	DB_DATA_TYPE::DB_UCHAR, 	1, NULL},
	{NULL ,		DB_DATA_TYPE::DB_INVALID, 	0, NULL}
};
*/

struct testDataStruct
{
	uint64_t userId;
	char name[32];
	unsigned char isOk;
} __attribute__ ((packed));


#endif
