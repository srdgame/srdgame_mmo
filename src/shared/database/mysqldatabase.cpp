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
		MySQLConnection* con = (MySQLConnection*)_connection[i];
		mysql_close(con->MySql);
		delete con;
	}
	_connections.clear();
}

bool MySQLDatabase::open(const DatabaseConn& conn)
{
	_conn = conn;
	LogNotice(DB_NAME, "Connecting to '%s' : '%d', database: '%s' username: '%s' password:*******", 
			conn._host_ip, conn._host_port, conn._database, conn._user_name);
	size_t con_size = 0;
	MYSQL* con;
	MYSQL* real_con;
	bool recon = true;
	for (; con_size < MYSQL_CONNECTION_COUNT; ++con_size)
	{
		con = mysql_init(NULL);
		if (!con)
			continue;
		if (mysql_options(con, MYSQL_SET_CHARSET_NAME, "utf8"))
		{
			LogError(DB_NAME, "Could not set character set to be utf8");
		}

		if (mysql_options(con, MYSQL_OPT_RECONNECT, &recon))
		{
			LogError(DB_NAME, "MYSQL_OPT_RECONNECT could not be set, connection drops may occur but will be counteracted.");
		}

		real_con = mysql_real_connect( con, _conn._host_ip, _conn._username, _conn._password, _conn._databaseName, _conn._host_port, NULL, 0 );
		if (real_con == NULL )
		{
			LogError("MySQLDatabase", "Connection failed due to: `%s`", mysql_error( con ) );
			mysql_close(con);
			return false;
		}

		MySQLConnection* new_con = new MySQLConnection;
		new_con->MySql = temp2;
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
