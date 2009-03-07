#include "database.h"
#include "queryresult.h"
#include "dbconn.h"

//#if PLATFORM == PLATFORM_APPLE
//#include <mysql.h>
//#else
#include <mysql/mysql.h>
//#endif

#include <vector>

namespace srdgame
{
class MySQLConn : public DBConn
{
public:
	MYSQL* MySql;
};
class MySQLQueryResult : public QueryResult
{
	friend class MySQLDatabase;
protected:
	MySQLQueryResult() : QueryResult(0, 0){}
public:
	bool next();
	
protected:

};
class MySQLDatabase : public Database
{
	friend class DBManager;
	virtual ~MySQLDatabase();
public:
	bool open(const DatabaseConn& conn);
	void close() = 0;

	QueryResult* query(const char* sql, ...);
	bool execute(const char* sql, ...);
private:
};
};
