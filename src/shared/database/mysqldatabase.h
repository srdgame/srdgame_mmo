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
struct MySQLConn : public DBConn
{
public:
	MYSQL* _mysql;
};
class MySQLQueryResult : public QueryResult
{
	friend class MySQLDatabase;
protected:
	MySQLQueryResult(MYSQL_RES* result, size_t field_size, size_t row_size);
	~MySQLQueryResult();
public:
	bool next();
	
protected:
	MYSQL_RES* _result;
};
class MySQLDatabase : public Database
{
	friend class DBManager;
	virtual ~MySQLDatabase();
public:
	bool open(const DatabaseInfo& info);
	void close();
protected:
	bool send_query(DBConn* conn, const char* sql);
	QueryResult* get_result(DBConn* conn);
private:
};
};
