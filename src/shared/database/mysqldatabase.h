#include "database.h"
#include "queryresult.h"

#if PLATFORM == PLATFORM_APPLE
#include <mysql.h>
#else
#include <mysql/mysql.h>
#endif

namespace srdgame
{
class MySQLConnection : DBConnection
{
	MYSQL* MySql;
};
class MySQLQueryResult : public QueryResult
{
	friend class MySQLDatabase;
protected:
	MySQLQueryResult(){}
public:
	bool next();
	
protected:

};
class MySQLDatabase : public Database
{
public:
	bool open(const DatabaseConn& conn);
	void close() = 0;

	QueryResult* query(const char* sql, ...);
	bool execute(const char* sql, ...);
};
};
