/*************************************************************************
  > File Name: test.cpp
  > Author: huangyun
  > Mail: 895175589@qq.com 
  > Created Time: Fri 26 Jun 2015 11:21:20 PM
 ************************************************************************/

#include<iostream>
#include "DBPool.h"
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <cstring>
#include <atomic>
#include <sstream>

#include "DBDefine.h"

bool bExit = 0;

std::atomic_uint insertIndex(0);
std::atomic_uint deleteIndex(0);
std::atomic_uint updateIndex(0);
std::atomic_uint selectIndex(0);

void signal_ctrlc(int sig)
{
//	std::cout<<"get signal:"<<sig<<std::endl;
	bExit = 1;
}

void* funInsert(void *arg)
{
	pthread_detach(pthread_self());
	MyNameSpace::DBPool * pool = (MyNameSpace::DBPool *)arg;
	int id = pool->getConn();
//	std::cout<<"pthread :"<<pthread_self()<<"get Handle:"<<id<<"......operate db in here!"<<std::endl;
	testDataStruct temp;
	memset(&temp, 0x00, sizeof(temp));
	temp.userId = insertIndex++;
	snprintf(temp.name, sizeof(temp.name), "%llu", temp.userId);
	temp.isOk = '1';
	const dbCol column[] = 
	{
		{"USERID",  DB_DATA_TYPE::DB_ULONG,     8, (unsigned char *)(&(temp.userId))},
		{"MYNAME",  DB_DATA_TYPE::DB_STR,       32, (unsigned char *)(temp.name)},
		{"ISOK",    DB_DATA_TYPE::DB_UCHAR,     1, (unsigned char *)(&(temp.isOk))},
		{NULL ,     DB_DATA_TYPE::DB_INVALID,   0, NULL}
	};
	if ((uint32_t)-1 != pool->execInsert(id, "USERINFO", column))
	{
		std::cout<<"execInsert Success"<<std::endl;
	}
	else
	{
		std::cout<<"execInsert Fail"<<std::endl;
	}
	sleep(2);
	pool->releaseConn(id);
	return NULL;
}

void* funDelete(void *arg)
{
	pthread_detach(pthread_self());
	MyNameSpace::DBPool * pool = (MyNameSpace::DBPool *)arg;
	int id = pool->getConn();
//	std::cout<<"pthread :"<<pthread_self()<<"get Handle:"<<id<<"......operate db in here!"<<std::endl;
	std::ostringstream where;
	where << " USERID = " << deleteIndex++;
	if ((uint32_t)-1 != pool->execDelete(id, "USERINFO", where.str().c_str()))
	{
		std::cout<<"execDelete Success"<<std::endl;
	}
	else
	{
		std::cout<<"execDelete Fail"<<std::endl;
	}
	sleep(2);
	pool->releaseConn(id);
	return NULL;
}


void* funUpdate(void *arg)
{
	pthread_detach(pthread_self());
	MyNameSpace::DBPool * pool = (MyNameSpace::DBPool *)arg;
	int id = pool->getConn();
	//	std::cout<<"pthread :"<<pthread_self()<<"get Handle:"<<id<<"......operate db in here!"<<std::endl;
	uint32_t index = updateIndex++;
	std::ostringstream where;
	where << " USERID = " << index;
	testDataStruct temp;
	memset(&temp, 0x00, sizeof(temp));
	snprintf(temp.name, sizeof(temp.name), "update:%u", index);
	temp.isOk = '0';
	const dbCol column[] = 
	{
		{"MYNAME",  DB_DATA_TYPE::DB_STR,       32, (unsigned char *)(temp.name)},
		{"ISOK",    DB_DATA_TYPE::DB_UCHAR,     1, (unsigned char *)(&(temp.isOk))},
		{NULL ,     DB_DATA_TYPE::DB_INVALID,   0, NULL}
	};

	if ((uint32_t)-1 != pool->execUpdate(id, "USERINFO", column, where.str().c_str()))
	{
		std::cout<<"execUpdate Success"<<std::endl;
	}
	else
	{
		std::cout<<"execUpdate Fail"<<std::endl;
	}

	sleep(2);
	pool->releaseConn(id);
	return NULL;
}

void* funSelect(void *arg)
{
	pthread_detach(pthread_self());
	MyNameSpace::DBPool * pool = (MyNameSpace::DBPool *)arg;
	int id = pool->getConn();
//	std::cout<<"pthread :"<<pthread_self()<<"get Handle:"<<id<<"......operate db in here!"<<std::endl;
	const dbCol column[] = 
	{
		{"USERID",  DB_DATA_TYPE::DB_ULONG,     8, NULL},
		{"MYNAME",  DB_DATA_TYPE::DB_STR,       32, NULL},
		{"ISOK",    DB_DATA_TYPE::DB_UCHAR,     1, NULL},
		{NULL ,     DB_DATA_TYPE::DB_INVALID,   0, NULL}
	};
//	std::cout<<sizeof(testDataStruct)<<std::endl;
	uint32_t retCount = 0;
#define SELECT_LIMIT
#ifdef SELECT_LIMIT
#define SELECT_COUNT 10
	testDataStruct data[SELECT_COUNT];
	memset(data, 0x00, sizeof(data));
	retCount = pool->execSelectLimit(id, "USERINFO", column, NULL, NULL, SELECT_COUNT, (unsigned char *)data);
#else
	testDataStruct *data = NULL;
	retCount = pool->execSelect(id, "USERINFO", column, NULL, NULL, (unsigned char **)&data);
#endif

	if ((uint32_t)-1 == retCount)
	{
		std::cout<<"execSelect error"<<std::endl;
	}
	else
	{
#ifdef SELECT_LIMIT
		for (uint32_t i = 0; i < SELECT_COUNT; ++i)
		{
			std::cout<<"userid: "<<data[i].userId<<std::endl;
			std::cout<<"name: "<<data[i].name<<std::endl;
			std::cout<<"isOk: "<<data[i].isOk<<std::endl;
		}
#else
		for (uint32_t i = 0; i < retCount; ++i)
		{
			std::cout<<"userid: "<<data[i].userId<<std::endl;
			std::cout<<"name: "<<data[i].name<<std::endl;
			std::cout<<"isOk: "<<data[i].isOk<<std::endl;
		}
		delete [] data;
#endif
	}
	sleep(2);
	pool->releaseConn(id);
	return NULL;
}

int main()
{
	MyNameSpace::DBPool::DBConnInfo info;
	info.host = "localhost";
	info.user = "root";
	info.passwd = "123123";
	info.dbName = "test";
	info.port = 3306;
	MyNameSpace::DBPool pool(info);
	signal(SIGINT, signal_ctrlc);
	pthread_t pid1;
	pthread_t pid2;
	pthread_t pid3;
	pthread_t pid4;
	//ctrl+c exit loop
	while(!bExit)
	{
		pthread_create(&pid1, NULL, funInsert, (void *)&pool);
		pthread_create(&pid2, NULL, funDelete, (void *)&pool);
		pthread_create(&pid3, NULL, funUpdate, (void *)&pool);
		pthread_create(&pid4, NULL, funSelect, (void *)&pool);
		sleep(1);
	}
	return 0;
}
