/*************************************************************************
  > File Name: db.cpp
  > Author: huangyun
  > Mail: 895175589@qq.com 
  > Created Time: Fri 26 Jun 2015 09:34:21 PM
 ************************************************************************/

#include<iostream>
#include <mysql/mysql.h>
#include <pthread.h>
#include "MyTime.h"
#include "MyLock.h"
#include "DBPool.h"
#include <unistd.h>
#include <cstring>
#include <sstream>

namespace MyNameSpace
{
	class MyDBConn
	{
		public:
			enum CONN_STATUS
			{
				CONN_INVALID,
				CONN_USING,
				CONN_VALID,
			};
			MyDBConn(DBPool::DBConnInfo &info) : connInfo(info), timeout(60), 
			mysql(NULL), myId(0), connStatus(CONN_INVALID),
			threadId(0), supportTransactions(false)
		{
		}
			~MyDBConn()
			{
				fini();
			}
			bool init()
			{
				if (mysql)
				{
					mysql_close(mysql);
					mysql = NULL;
				}
				if (connDB())
				{
					supportTransactions = connInfo.supportTransactions;
					return true;
				}
				std::cerr<<"init DB error"<<std::endl;
				return false;
			}
			void fini()
			{
				if (mysql)
				{
					mysql_close(mysql);
				}
			}
			void getConn()
			{
				threadId = pthread_self();
				std::cout<<"get ("<<threadId<<":"<<myId<<")"<<std::endl;
				connStatus = CONN_USING;
				myTime.now();
			}
			void releaseConn()
			{
				std::cout<<"release ("<<threadId<<":"<<myId<<")"<<std::endl;
				threadId = 0;
				connStatus = CONN_VALID;
			}
			time_t getUsingTime()
			{
				return myTime.elapse();
			}

			int getMyId()
			{
				return myId;
			}

			CONN_STATUS getConnStatus()
			{
				return connStatus;
			}

			pthread_t getThreadId()
			{
				return threadId;
			}

			int execSql(const char *sql, uint32_t sqlLen)
			{
				if (NULL == sql || 0 == sqlLen || NULL == mysql)
				{
					std::cerr<<__FUNCTION__<<": "<<__LINE__<<"exec sql error sqlLen:"<<sqlLen<<" sql statement:"<<sql<<std::endl;
					return -1;
				}

				int ret = mysql_real_query(mysql, sql, sqlLen);
				if (0 != ret)
				{
					std::cerr<<__FUNCTION__<<": "<<__LINE__<<"sql exec error: "<<sql<<std::endl;
					std::cerr<<mysql_error(mysql)<<std::endl;
				}
				return ret;
			}

			uint32_t execInsert(const char *tableName, const dbCol *column)
			{
				if (NULL == tableName || NULL == column || NULL == mysql)
				{
					std::cerr<<__FUNCTION__<<": "<<__LINE__<<"execInsert error"<<std::endl;
					return (uint32_t)-1;
				}
				const dbCol *temp = NULL;
				std::ostringstream strSql;
				strSql << "INSERT INTO ";
				strSql << tableName;
				strSql << "(";
				temp = column;
				bool first = true;
				while (NULL != temp->name)
				{
					uint32_t len = ::strlen(temp->name);
					if (len > 0)
					{
						if (first)
						{
							first = false;
						}
						else
						{
							strSql << ", ";
						}
					}
					strSql << temp->name;
					++temp;
				}

				strSql << ") VALUES(";
				first = true;
				temp = column;

				while (NULL != temp->name)
				{
					if (::strlen(temp->name) > 0)
					{
						if (first)
						{
							first = false;
						}
						else
						{
							strSql << ",";
						}
						switch(temp->type)
						{
							case DB_DATA_TYPE::DB_CHAR:
								{
									int8_t c = *(int8_t *)temp->data;
									strSql << c;
								}
								break;
							case DB_DATA_TYPE::DB_UCHAR:
								{
									uint8_t uc = *(uint8_t *)temp->data;
									strSql << uc;
								}
								break;
							case DB_DATA_TYPE::DB_SHORT:
								{
									int16_t s = *(int16_t *)temp->data;
									strSql << s;
								}
								break;
							case DB_DATA_TYPE::DB_USHORT:
								{
									uint16_t us = *(uint16_t *)temp->data;
									strSql << us;
								}
								break;
							case DB_DATA_TYPE::DB_INT:
								{
									int32_t i = *(int32_t *)temp->data;
									strSql << i;
								}
								break;
							case DB_DATA_TYPE::DB_UINT:
								{
									uint32_t ui = *(uint32_t *)temp->data;
									strSql << ui;
								}
								break;
							case DB_DATA_TYPE::DB_LONG:
								{
									int64_t l = *(int64_t *)temp->data;
									strSql << l;
								}
								break;
							case DB_DATA_TYPE::DB_ULONG:
								{
									uint64_t ul = *(uint64_t *)temp->data;
									strSql << ul;
								}
								break;
							case DB_DATA_TYPE::DB_STR:
								{
									char strData[2 * temp->size + 1] = {0};
									mysql_real_escape_string(mysql, strData, (const char*&)temp->data, temp->size);
									strSql << "\'" << strData << "\'";
								}
								break;
							case DB_DATA_TYPE::DB_BIN:
								{
									char strData[2 * temp->size + 1] = {0};
									mysql_real_escape_string(mysql, strData, (const char *)temp->data, temp->size);
									strSql << "\'" << strData << "\'";
								}
								break;
							default:
								{
									std::cerr<<__FUNCTION__<<": "<<__LINE__<<"sql type: "<<(int)temp->type<<" error"<<std::endl;
								}
								break;
						}
					}
					++temp;
				}
				strSql << ")";
//				std::cout<<strSql.str()<<std::endl;
				if (0 == execSql(strSql.str().c_str(), strSql.str().size()))
				{
					return (uint32_t)mysql_insert_id(mysql);
				}
				else
				{
					return (uint32_t)-1;
				}
			}

			uint32_t execDelete(const char *tableName, const char * where)
			{
				if (NULL == tableName || NULL == mysql)
				{
					std::cerr<<__FUNCTION__<<": "<<__LINE__<<" execDelete error nullptr"<<std::endl;
				}
				std::string strSql = "DELETE FROM ";
				strSql += tableName;
				if (where)
				{
					strSql += " WHERE ";
					strSql += where;
				}
				if (0 == execSql(strSql.c_str(), strSql.length()))
				{
					return (uint32_t)mysql_affected_rows(mysql);
				}
				else
				{
					return (uint32_t)-1;
				}
			}

			uint32_t execUpdate(const char *tableName, const dbCol *column, const char * where)
			{
				if (NULL == tableName || NULL == column || NULL == mysql)
				{
					std::cerr<<__FUNCTION__<<": "<<__LINE__<<" execUpdate error nullptr"<<std::endl;
				}
				std::ostringstream strSql;
				strSql << "UPDATE " << tableName << " SET ";
				const dbCol * temp = NULL;
				temp = column;
				bool first = true;
				while (NULL != temp->name)
				{
					uint32_t len = ::strlen(temp->name);
					if (len > 0)
					{

						if (first)
						{
							first = false;
						}
						else
						{
							strSql << ", ";
						}
						strSql << temp->name << "=";
						switch(temp->type)
						{
							case DB_DATA_TYPE::DB_CHAR:
								{
									char c = *(char *)temp->data;
									strSql << c;
								}
								break;
							case DB_DATA_TYPE::DB_UCHAR:
								{
									unsigned char uc = *(unsigned char *)temp->data;
									strSql << uc;
								}
								break;
							case DB_DATA_TYPE::DB_SHORT:
								{
									int16_t s = *(int16_t *)temp->data;
									strSql << s;
								}
								break;
							case DB_DATA_TYPE::DB_USHORT:
								{
									uint16_t us = *(uint16_t *)temp->data;
									strSql << us;
								}
								break;
							case DB_DATA_TYPE::DB_INT:
								{
									int32_t i = *(int32_t *)temp->data;
									strSql << i;
								}
								break;
							case DB_DATA_TYPE::DB_UINT:
								{
									uint32_t ui = *(uint32_t *)temp->data;
									strSql << ui;
								}
								break;
							case DB_DATA_TYPE::DB_LONG:
								{
									int64_t l = *(int64_t *)temp->data;
									strSql << l;
								}
								break;
							case DB_DATA_TYPE::DB_ULONG:
								{
									uint64_t ul = *(uint64_t *)temp->data;
									strSql << ul;
								}
								break;
							case DB_DATA_TYPE::DB_STR:
								{
									char strData[2 * temp->size + 1] = {0};
									mysql_real_escape_string(mysql, strData, (const char*&)temp->data, temp->size);
									strSql << "\'" << strData << "\'";
								}
								break;
							case DB_DATA_TYPE::DB_BIN:
								{
									char strData[2 * temp->size + 1] = {0};
									mysql_real_escape_string(mysql, strData, (const char *)temp->data, temp->size);
									strSql << "\'" << strData << "\'";
								}
								break;
							default:
								{
									std::cerr<<__FUNCTION__<<": "<<__LINE__<<"sql type: "<<(int)temp->type<<" error"<<std::endl;
								}
								break;
						}
					}
					++temp;
				}
				if (NULL != where)
				{
					strSql << " WHERE " <<where;
				}
				if (0 == execSql(strSql.str().c_str(), strSql.str().size()))
				{
					return (uint32_t)mysql_affected_rows(mysql);
				}
				else
				{
					return (uint32_t)-1;
				}
			}

			uint32_t execSelect(const char *tableName, const dbCol *column, const char *where, const char *order, unsigned char **data)
			{
				uint32_t retVal = -1;
				uint32_t retSize = 0;
				*data = NULL;
				if (NULL == tableName || NULL == column || NULL == mysql)
				{
					std::cerr<<__FUNCTION__<<": "<<__LINE__<<"nullptr error"<<std::endl;
				}
				retSize = fetchSelectSql(tableName, column, where, order);
				if ((uint32_t)-1 == retSize)
				{
					return retVal;
				}
				uint32_t retCount = 0;
				MYSQL_RES *result = NULL;
				{
					result = mysql_store_result(mysql);
					if (NULL == result)
					{
						std::cerr<<__FUNCTION__<<": "<<__LINE__<<"sql error"<<mysql_error(mysql)<<std::endl;
						return retVal;
					}
					retCount = mysql_num_rows(result);
					if (0 == retCount)
					{
						mysql_free_result(result);
						return retCount;
					}
					*data = new unsigned char[retCount * retSize];
					if (NULL == *data)
					{
						std::cerr<<__FUNCTION__<<": "<<__LINE__<<"new error"<<std::endl;
						mysql_free_result(result);
						return retVal;
					}

					bzero(*data, retCount * retSize);

					MYSQL_ROW row;
					unsigned char *tempData = *data;

					while (NULL != (row = mysql_fetch_row(result)))
					{
						unsigned long *lengths = mysql_fetch_lengths(result);
						uint32_t fullSize = fullSelectDataByRow(row, lengths, column, tempData);
						if (0 == fullSize)
						{
							delete [] (*data);
							mysql_free_result(result);
							return retVal;
						}
						tempData += fullSize;
					}
				}
				mysql_free_result(result);
				return retCount;
			}

			uint32_t execSelectLimit(const char *tableName, const dbCol *column, const char *where, const char *order, uint32_t limit, unsigned char *data, uint32_t limitFrom = 0)
			{
				uint32_t retError = (uint32_t)-1;
				uint32_t retSize = 0;
				if (NULL == tableName || NULL == column || NULL == mysql)
				{
					std::cerr<<__FUNCTION__<<": "<<__LINE__<<"nullptr error"<<std::endl;
					return retError;
				}
				retSize = fetchSelectSql(tableName, column, where, order, limit, limitFrom);
				if ((uint32_t)-1 == retSize)
				{
					return retError;
				}
				uint32_t retCount = 0;
				MYSQL_RES *result = NULL;
				{
					result = mysql_store_result(mysql);
					if (NULL ==result)
					{
						std::cerr<<__FUNCTION__<<": "<<__LINE__<<"sql error"<<mysql_error(mysql)<<std::endl;
						return retError;
					}
					retCount = mysql_num_rows(result);
					if (0 == retCount)
					{
						mysql_free_result(result);
						return retCount;
					}
				}

				MYSQL_ROW row;
				unsigned char *tempData = data;
				uint32_t count = 0;
				while ((NULL != (row = mysql_fetch_row(result))) && count < limit)
				{
					unsigned long *lengths = mysql_fetch_lengths(result);
					uint32_t fullSize = fullSelectDataByRow(row, lengths, column, tempData);
					if (0 == fullSize)
					{
						mysql_free_result(result);
						return count;
					}
					tempData += fullSize;
					++count;
				}
				mysql_free_result(result);
				return count;
			}

			bool setTransactions(bool bSupportTransactions)
			{
				if (bSupportTransactions)
				{
					return (0 == execSql(withTransactionsCmd.c_str(), withTransactionsCmd.length()));
				}
				else
				{
					return (0 == execSql(withoutTransactionsCmd.c_str(), withoutTransactionsCmd.length()));
				}
			}

			bool isSupportTransactions()
			{
				return supportTransactions;
			}

		private:
			uint32_t fetchSelectSql(const char *tableName, const dbCol *column, const char *where, const char *order, uint32_t limit = 0, uint32_t limitFrom = 0)
			{
				uint32_t retSize = 0;
				const dbCol *temp = NULL;
				bool first = true;
				std::string sql;
				temp = column;

				sql += "SELECT ";
				while (NULL != temp->name)
				{
					retSize += temp->size;
					if (::strlen(temp->name) > 0)
					{
						if (first)
						{
							first = false;
						}
						else
						{
							sql += ", ";
						}
						sql += temp->name;
					}
					++temp;
				}
				
				if (::strlen(tableName) > 0)
				{
					sql += " FROM ";
					sql += tableName;
				}

				if (NULL != where && ::strlen(where) > 0)
				{
					sql += " WHERE ";
					sql += where;
				}

				if (NULL != order && ::strlen(order) > 0)
				{
					sql += " ORDER BY ";
					sql += order;
				}
				if (limitFrom)
				{
					char tmp[32] = {0};
					snprintf(tmp, sizeof(tmp) - 1, "%u, %u", limitFrom, limit);
					sql += " LIMIT ";
					sql += tmp;
				}
				else if (limit)
				{
					char tmp[32] = {0};
					snprintf(tmp, sizeof(tmp) - 1, "%u", limit);
					sql += " LIMIT ";
					sql += tmp;
				}

//				std::cerr<<"fetchSelectSql:"<<sql<<std::endl;
				if (0 == execSql(sql.c_str(), sql.length()))
				{
					return retSize;
				}
				else
				{
					return (uint32_t)-1;
				}
			}

			uint32_t fullSelectDataByRow(MYSQL_ROW row, unsigned long *lengths, const dbCol *temp, unsigned char *tempData)
			{
				uint32_t offset = 0;
				uint32_t i = 0;
				while (NULL != temp->name)
				{
					if (::strlen(temp->name) > 0)
					{
						switch(temp->type)
						{
							case DB_DATA_TYPE::DB_CHAR:
								{
									if (row[i])
									{
										*(char *)(tempData + offset) = *row[i];
									}
									else
									{
										*(char *)(tempData + offset) = 0;
									}
								}
								break;
							case DB_DATA_TYPE::DB_UCHAR:
								{
									if (row[i])
									{
										*(unsigned char *)(tempData + offset) = *row[i];
									}
									else
									{
										*(unsigned char *)(tempData + offset) = 0;
									}
								}
								break;
							case DB_DATA_TYPE::DB_SHORT:
								{
									if (row[i])
									{
										*(int16_t *)(tempData + offset) = ::strtoul(row[i], (char **)NULL, 10);
									}
									else
									{
										*(int16_t *)(tempData + offset) = 0;
									}
								}
								break;
							case DB_DATA_TYPE::DB_USHORT:
								{
									if (row[i])
									{
										*(uint16_t *)(tempData + offset) = ::strtoul(row[i], (char **)NULL, 10);
									}
									else
									{
										*(uint16_t *)(tempData + offset) = 0;
									}
								}
								break;
							case DB_DATA_TYPE::DB_INT:
								{
									if (row[i])
									{
										*(int32_t *)(tempData + offset) = ::strtoul(row[i], (char **)NULL, 10);
									}
									else
									{
										*(int32_t *)(tempData + offset) = 0;
									}
								}
								break;
							case DB_DATA_TYPE::DB_UINT:
								{
									if (row[i])
									{
										*(uint32_t *)(tempData + offset) = ::strtoul(row[i], (char **)NULL, 10);
									}
									else
									{
										*(uint32_t *)(tempData + offset) = 0;
									}
								}
								break;
							case DB_DATA_TYPE::DB_LONG:
								{
									if (row[i])
									{
										*(int64_t *)(tempData + offset) = ::strtoull(row[i], (char **)NULL, 10);
									}
									else
									{
										*(int64_t *)(tempData + offset) = 0;
									}
								}
								break;
							case DB_DATA_TYPE::DB_ULONG:
								{
									if (row[i])
									{
										*(uint64_t *)(tempData + offset) = ::strtoull(row[i], (char **)NULL, 10);
									}
									else
									{
										*(uint64_t *)(tempData + offset) = 0;
									}
								}
								break;
							case DB_DATA_TYPE::DB_STR:
							case DB_DATA_TYPE::DB_BIN:
								{
									bzero(tempData + offset, temp->size);
									if (row[i])
									{
										bcopy(row[i], tempData + offset, temp->size > lengths[i] ? lengths[i] : temp->size);
									}
								}
								break;
							default:
								{
									std::cerr<<__FUNCTION__<<": "<<__LINE__<<"sql type: "<<(int)temp->type<<" error"<<std::endl;
								}
								break;
						}
//						std::cout<<"lengths:"<<lengths[i]<<"offset:"<<offset<<"temp->size:"<<temp->size<<"row[i]"<<row[i]<<std::endl;
						++i;
					}
					offset += temp->size;
					++temp;
				}
				return offset;
			}

		private:
			bool connDB()
			{
				mysql = mysql_init(NULL);
				if (NULL == mysql)
				{
					std::cerr<<"mysql init error:"<<std::endl;
					return false;
				}
				mysql_options(mysql, MYSQL_OPT_READ_TIMEOUT, (const char *)&timeout);
				mysql_options(mysql, MYSQL_OPT_WRITE_TIMEOUT, (const char *)&timeout);

				int opt = 1;
				mysql_options(mysql, MYSQL_OPT_RECONNECT, (char *)&opt);
				if (!mysql_real_connect(mysql, connInfo.host.c_str(), connInfo.user.c_str(), connInfo.passwd.c_str(), connInfo.dbName.c_str(), connInfo.port, 0, 0))
				{
					std::cerr<<"mysql connect error:"<<mysql_error(mysql)<<std::endl;
					mysql_close(mysql);
					mysql = NULL;
					return false;
				}
				myId = connId++;
				connStatus = CONN_VALID;
				std::cout<<"DB:"<<myId<<" conn ok!"<<std::endl;
				return true;
			}
		private:
			static int connId;
			DBPool::DBConnInfo connInfo;
			const int timeout;
			MYSQL *mysql;
			int myId;
			CONN_STATUS connStatus;
			pthread_t threadId;
			MyTime myTime;
			bool supportTransactions;
			static const std::string withoutTransactionsCmd;
			static const std::string withTransactionsCmd;
	};
	const std::string MyDBConn::withoutTransactionsCmd = "SET AUTOCOMMIT = 1";
	const std::string MyDBConn::withTransactionsCmd = "SET AUTOCOMMIT = 0";

	int MyDBConn::connId = 0; 

	int DBPool::getConn()
	{
		while(1)
		{
			MyScopeLock scopeLock(myLock);
			std::shared_ptr<MyDBConn> invalidDb;
			for(ConnPoolType::iterator iter = connPool.begin(); iter != connPool.end(); )
			{
				std::shared_ptr<MyDBConn> tmp;
				tmp = iter->second;
				if (!tmp)
				{
					connPool.erase(iter++);
					continue;
				}
				switch(tmp->getConnStatus())
				{
					case MyDBConn::CONN_INVALID:
						{
							invalidDb = tmp;
						}
						break;
					case MyDBConn::CONN_USING:
						{
							if (tmp->getUsingTime() > 600)
							{
								std::cerr<<"thread:"<<tmp->getThreadId()<<" using("<<tmp->getMyId()<<")"<<tmp->getUsingTime()<<"s"<<std::endl;
							}
						}
						break;
					case MyDBConn::CONN_VALID:
						{
							tmp->getConn();
							return tmp->getMyId();
						}
						break;
				}
				++iter;
			}
			if (NULL != invalidDb)
			{
				if (invalidDb->init())
				{
					invalidDb->getConn();
					return invalidDb->getMyId();
				}
				else
				{
					std::cerr<<"conn init error"<<std::endl;
				}
			}
			if (connPool.size() < MAX_CONN_COUNT)
			{
				std::shared_ptr<MyDBConn> pConn(new MyDBConn(connInfo));
				if (pConn)
				{
					if (!pConn->init())
					{
						std::cerr<<"conn init error"<<std::endl;
					}
					else
					{
						if (connPool.insert(ConnPoolType::value_type(pConn->getMyId(), pConn)).second)
						{
							pConn->getConn();
							return pConn->getMyId();
						}
						else
						{
							std::cerr<<"conn insert pool error"<<std::endl;
						}
					}
				}
			}
			std::cerr<<"get db conn usleep"<<std::endl;
			usleep(1000);
		}
	}

	std::shared_ptr<MyDBConn> DBPool::getHandleById(uint32_t handleId)
	{
		ConnPoolType::iterator iter = connPool.find(handleId);		
		if (iter != connPool.end())
		{
			return iter->second;
		}
		std::shared_ptr<MyDBConn> retNull;
		return retNull;
	}

	void DBPool::releaseConn(int connId)
	{
		std::shared_ptr<MyDBConn> ret = getHandleById(connId);
		if (!ret)
		{
			std::cerr<<"release conn error not found("<<connId<<"in pool"<<std::endl;
		}
		ret->releaseConn();
	}
	uint32_t DBPool::execInsert(uint32_t handleId, const char *tableName, const dbCol *column)
	{
		std::shared_ptr<MyDBConn> ret = getHandleById(handleId);
		if (ret)
		{
			return ret->execInsert(tableName, column);
		}
		else
		{
			return (uint32_t)-1;
		}
	}
	uint32_t DBPool::execDelete(uint32_t handleId, const char *tableName, const char * where)
	{
		std::shared_ptr<MyDBConn> ret = getHandleById(handleId);
		if (ret)
		{
			return ret->execDelete(tableName, where);
		}
		else
		{
			return (uint32_t)-1;
		}
	}
	uint32_t DBPool::execUpdate(uint32_t handleId, const char *tableName, const dbCol *column, const char * where)
	{
		std::shared_ptr<MyDBConn> ret = getHandleById(handleId);
		if (ret)
		{
			return ret->execUpdate(tableName, column, where);
		}
		else
		{
			return (uint32_t)-1;
		}
	}
	uint32_t DBPool::execSelect(uint32_t handleId, const char *tableName, const dbCol *column, const char *where, const char *order, unsigned char **data)
	{
		std::shared_ptr<MyDBConn> ret = getHandleById(handleId);
		if (ret)
		{
			return ret->execSelect(tableName, column, where, order, data);
		}
		else
		{
			return (uint32_t)-1;
		}
	}
	uint32_t DBPool::execSelectLimit(uint32_t handleId, const char *tableName, const dbCol *column, const char *where, const char *order, uint32_t limit, unsigned char *data, uint32_t limitFrom)
	{
		std::shared_ptr<MyDBConn> ret = getHandleById(handleId);
		if (ret)
		{
			return ret->execSelectLimit(tableName, column, where, order, limit, data, limitFrom);
		}
		else
		{
			return (uint32_t)-1;
		}
	}
	bool DBPool::commit(uint32_t handleId)
	{
		std::shared_ptr<MyDBConn> ret = getHandleById(handleId);
		if (ret)
		{
			return (0 == ret->execSql("COMMIT", strlen("COMMIT")));
		}
		else
		{
			return false;
		}
	}
	bool DBPool::rollback(uint32_t handleId)
	{
		std::shared_ptr<MyDBConn> ret = getHandleById(handleId);
		if (ret)
		{
			return (0 == ret->execSql("ROLLBACK", strlen("ROLLBACK")));
		}
		else
		{
			return false;
		}
	}
	bool DBPool::setTransactions(uint32_t handleId, bool bSupportTransactions)
	{
		std::shared_ptr<MyDBConn> ret = getHandleById(handleId);
		if (ret)
		{
			return ret->setTransactions(bSupportTransactions);
		}
		else
		{
			return false;
		}
	}
	bool DBPool::isSupportTransactions(uint32_t handleId)
	{
		std::shared_ptr<MyDBConn> ret = getHandleById(handleId);
		if (ret)
		{
			return ret->isSupportTransactions();
		}
		else
		{
			return false;
		}
	}
}
