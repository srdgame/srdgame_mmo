#ifndef DATABASE_H_
#define DATABASE_H_

#include "threadbase.h"
#include <string>

#include <vector>
#include "fastqueue.h"

namespace srdgame
{
struct DatabaseConn
{
	std::string _host_ip;
	unsigned int _host_port;
	std::string _username;
	std::string _password;
	std::string _database;
};
class QueryResult;
class DBConnection;
class Database
{
	friend class DBManager;
protected:
	Database(){}
public:
	// databse methods
	virtual bool open(const DatabaseConn& conn) = 0;
	virtual void close() = 0;

	virtual QueryResult* query(const char* sql, ...) = 0;
	virtual bool execute(const char* sql, ...) = 0;


protected:
	DatabaseConn _conn;
	std::vector<DBConnection*> _connections;
	size_t _max_con_size;
	size_t _buf_size;
};
}

#endif
