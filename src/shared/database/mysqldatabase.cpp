#include "mysqldatabase.h"
#include "field.h"
#include "log.h"

using namespace srdgame;
#define MYSQL_CONNECTION_COUNT 64
#define MYSQL_BUFFER_SIZE 10240

#ifdef DB_NAME
#undef DB_NAME
#endif
#define DB_NAME "MySQL-Database"

MySQLQueryResult::MySQLQueryResult(MYSQL_RES* result, size_t field_size, size_t row_size)
	: QueryResult(field_size, row_size)
	  , _result(result)
{
}

MySQLQueryResult::~MySQLQueryResult()
{
	mysql_free_result(_result);
}

bool MySQLQueryResult::next()
{
	MYSQL_ROW row = mysql_fetch_row(_result);
	if (row)
	{
		for (unsigned long i = 0; i < _field_size; ++i)
		{
			_cur[i].setup(row[i]);
		}
		return true;
	}
	return false;
}

MySQLDatabase::~MySQLDatabase()
{
	for (size_t i = 0; i < _conns.size(); ++i)
	{
		MySQLConn* conn = (MySQLConn*)_conns[i];
		mysql_close(conn->_mysql);
		delete conn;
	}
	_conns.clear();
}

bool MySQLDatabase::open(const DatabaseInfo& info)
{
	_info = info;
	LogNotice(DB_NAME, "Connecting to '%s' : '%d', database: '%s' username: '%s' password:*******", 
			_info._host_ip.c_str(), _info._host_port, _info._database.c_str(), _info._username.c_str());
	size_t conn_size = 0;
	MYSQL* conn;
	MYSQL* real_conn;
	for (; conn_size < info._max_conn_count; ++conn_size)
	{
		conn = mysql_init(NULL);
		if (!conn)
			continue;
		/*if (mysql_options(conn, MYSQL_SET_CHARSET_NAME, "utf8"))
		{
			LogError(DB_NAME, "Could not set character set to be utf8");
		}*/
		if (mysql_options(conn, MYSQL_SET_CHARSET_NAME, "latin1"))
		{
			LogError(DB_NAME, "Could not set character set to be latin1");
		}

		if (mysql_options(conn, MYSQL_OPT_RECONNECT, "true"))
		{
			LogError(DB_NAME, "MYSQL_OPT_RECONNECT could not be set, connection drops may occur but will be counteracted.");
		}

		real_conn = mysql_real_connect( conn, _info._host_ip.c_str(), _info._username.c_str(), _info._password.c_str(), _info._database.c_str(), _info._host_port, NULL, 0 );
		if (real_conn == NULL )
		{
			LogError(DB_NAME, "Connection failed due to: `%s`", mysql_error( conn ) );
			mysql_close(conn);
			return false;
		}
		if (mysql_options(real_conn, MYSQL_SET_CHARSET_NAME, "utf8"))
		{
			LogError(DB_NAME, "Could not set character set to be utf8, in real connection");
		}

		MySQLConn* new_conn = new MySQLConn;
		new_conn->_mysql = real_conn;
		_conns.push_back(new_conn);
	}
}

void MySQLDatabase::close()
{
}

bool MySQLDatabase::send_query(DBConn* conn, const char* sql)
{
	MySQLConn* conn_ = static_cast<MySQLConn*>(conn);
	int res = mysql_query(conn_->_mysql, sql);
	if (res > 0)
	{
		LogError("MySQLDatabase", "Sql query failed due to [%s], Query: [%s]\n", mysql_error(conn_->_mysql ), sql);
	}

	return res == 0 ? true : false;
}
QueryResult* MySQLDatabase::get_result(DBConn* conn)
{
	MySQLConn* conn_ = static_cast<MySQLConn*>(conn);
	MYSQL_RES* res_ = mysql_store_result(conn_->_mysql);
	if (!res_)
		return NULL;

	unsigned long rows = mysql_affected_rows(conn_->_mysql);
	unsigned long fields = mysql_field_count(conn_->_mysql);

	MySQLQueryResult* res = NULL;

	if (rows && fields)
	{
		res = new MySQLQueryResult(res_, fields, rows);
		res->next();
	}
	else
	{
		mysql_free_result(res_);
	}
	return res;
}
