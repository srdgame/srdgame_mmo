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

bool MySQLQueryResult::next()
{
}

MySQLDatabase::~MySQLDatabase()
{
	for (size_t i = 0; i < _connections.size(); ++i)
	{
		MySQLConn* con = (MySQLConn*)_connections[i];
		mysql_close(con->MySql);
		delete con;
	}
	_connections.clear();
}

bool MySQLDatabase::open(const DatabaseInfo& info)
{
	_info = info;
	LogNotice(DB_NAME, "Connecting to '%s' : '%d', database: '%s' username: '%s' password:*******", 
			_info._host_ip.c_str(), _info._host_port, _info._database.c_str(), _info._username.c_str());
	size_t con_size = 0;
	MYSQL* con;
	MYSQL* real_con;
	for (; con_size < MYSQL_CONNECTION_COUNT; ++con_size)
	{
		con = mysql_init(NULL);
		if (!con)
			continue;
		if (mysql_options(con, MYSQL_SET_CHARSET_NAME, "utf8"))
		{
			LogError(DB_NAME, "Could not set character set to be utf8");
		}

		if (mysql_options(con, MYSQL_OPT_RECONNECT, "true"))
		{
			LogError(DB_NAME, "MYSQL_OPT_RECONNECT could not be set, connection drops may occur but will be counteracted.");
		}

		real_con = mysql_real_connect( con, _info._host_ip.c_str(), _info._username.c_str(), _info._password.c_str(), _info._database.c_str(), _info._host_port, NULL, 0 );
		if (real_con == NULL )
		{
			LogError("MySQLDatabase", "Conn failed due to: `%s`", mysql_error( con ) );
			mysql_close(con);
			return false;
		}

		MySQLConn* new_con = new MySQLConn;
		new_con->MySql = real_con;
		_connections.push_back(new_con);
	}
}

void MySQLDatabase::close()
{
}

QueryResult* MySQLDatabase::query(const char* sql, ...)
{
}
bool MySQLDatabase::execute(const char* sql, ...)
{
	return true;
}
