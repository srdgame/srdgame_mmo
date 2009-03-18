#include "database.h"
#include "log.h"
#include "dbconn.h"

using namespace srdgame;


QueryResult* Database::query(const char* sql, ...)
{
	char sql_str[MAX_SQL_STRING_SIZE];
	::memset(sql_str, 0, MAX_SQL_STRING_SIZE);
	va_list ap;
	va_start(ap, sql);
	vsnprintf(sql_str, MAX_SQL_STRING_SIZE, sql, ap);
	va_end(ap);

	QueryResult* res = NULL;
	DBConn* conn = get_free_conn();
	if (conn)
	{
		LogDebug("Database", "SQL: %s", sql_str);
		if (send_query(conn, sql))
		{
			res = get_result(conn);
		}
		conn->_lock.unlock();
	}
	else
	{
		LogError("Database", "No Free connections????????????????");
	}
	return res;
}

QueryResult* Database::query(const std::string& sql)
{
	QueryResult* res = NULL;
	DBConn* conn = get_free_conn();
	if (conn)
	{
		LogDebug("Database", "SQL: %s", sql.c_str());
		if (send_query(conn, sql.c_str()))
		{
			res = get_result(conn);
		}
	}
	else
	{
		LogError("Database", "No Free connections????????????????");
	}

	conn->_lock.unlock();
	return res;

}

DBConn* Database::get_free_conn()
{
	size_t i = 0;
	for (; i < _conns.size(); ++i)
	{
		if (_conns[i]->_lock.trylock())
			return _conns[i];
	}
	return NULL;
}

bool Database::execute(DBConn* conn, const char* sql)
{
	return send_query(conn, sql);
}
