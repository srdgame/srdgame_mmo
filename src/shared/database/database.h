#ifndef DATABASE_H_
#define DATABASE_H_

#include "threadbase.h"
#include <string>
#include <cstring>

#include <vector>
#include "fastqueue.h"

namespace srdgame
{

#define MAX_SQL_STRING_SIZE 16384

struct DatabaseInfo
{
	std::string _host_ip;
	unsigned int _host_port;
	std::string _username;
	std::string _password;
	std::string _database;
	unsigned int _max_conn_count;
};
class QueryResult;
class DBConn;
class Database
{
	friend class DatabaseMgr;
protected:
	Database() : _max_con_size(0), _buf_size(0){}
	// databse methods
	virtual bool open(const DatabaseInfo& info) = 0;
	virtual void close() = 0;
	virtual bool send_query(DBConn* conn, const char* sql) = 0;
	virtual QueryResult* get_result(DBConn* conn) = 0;

	virtual QueryResult* query(const char* sql, ...);
	virtual QueryResult* query(const std::string& sql);
	DBConn* get_free_conn();
	virtual bool execute(DBConn* conn, const char* sql);

protected:
	DatabaseInfo _info;
	std::vector<DBConn*> _conns;
	size_t _max_con_size;
	size_t _buf_size;
};
}

#endif
