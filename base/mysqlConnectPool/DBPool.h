/*************************************************************************
  > File Name: DbPool.h
  > Author: huangyun
  > Mail: 895175589@qq.com 
  > Created Time: Fri 26 Jun 2015 09:44:15 PM
 ************************************************************************/

#ifndef __DBPOOLH__
#define __DBPOOLH__

#include<iostream>
#include <string>
#include <map>
#include <stdint.h>
#include <memory>
#include "MyLock.h"

namespace MyNameSpace
{	
	enum class DB_DATA_TYPE
	{
		DB_CHAR,
		DB_UCHAR,
		DB_SHORT,
		DB_USHORT,
		DB_INT,
		DB_UINT,
		DB_LONG,
		DB_ULONG,
		DB_STR,
		DB_BIN,
		//add new type before DB_INVALID

		DB_INVALID
	};

	typedef struct
	{
		const char *name;
		DB_DATA_TYPE type;
		uint32_t size;
		unsigned char *data;
	} dbCol;

	class MyDBConn;
	class DBPool
	{
		public:
			struct DBConnInfo
			{
				std::string host;	
				std::string user;
				std::string passwd;
				std::string dbName;
				int port;
				bool supportTransactions;
			};

		public:
			DBPool(DBConnInfo & info) : connInfo(info), MAX_CONN_COUNT(100)
			{

			}

			void setConnInfo(DBConnInfo & info)
			{
				connInfo = info;
			}
			int getConn();
			void releaseConn(int connId);
			uint32_t execInsert(uint32_t handleId, const char *tableName, const dbCol *column);
			uint32_t execDelete(uint32_t handleId, const char *tableName, const char * where);
			uint32_t execUpdate(uint32_t handleId, const char *tableName, const dbCol *column, const char * where);
			uint32_t execSelect(uint32_t handleId, const char *tableName, const dbCol *column, const char *where, const char *order, unsigned char **data);
			uint32_t execSelectLimit(uint32_t handleId, const char *tableName, const dbCol *column, const char *where, const char *order, uint32_t limit, unsigned char *data, uint32_t limitFrom = 0);
			bool commit(uint32_t handleId);
			bool rollback(uint32_t handleId);
			bool setTransactions(uint32_t handleId, bool bSupportTransactions);
			bool isSupportTransactions(uint32_t handleId);

		private:
			std::shared_ptr<MyDBConn> getHandleById(uint32_t handleId);
		private:
			using ConnPoolType = std::map<int, std::shared_ptr<MyDBConn> >;
			ConnPoolType connPool;
			DBConnInfo connInfo;
			const uint32_t MAX_CONN_COUNT;
			MyLock myLock;
	};
}

#endif
