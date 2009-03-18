#include "databasemgr.h"
#include "database.h"
#include "configfile.h"
#include "mysqldatabase.h"
#include "log.h"
#include "autolock.h"
#include "threadpool.h"
#include <cstring>

using namespace srdgame;
using namespace std;

DatabaseMgr::Worker::Worker(DatabaseMgr* mgr)
	: _mgr(mgr), _running(false)
{
}

bool DatabaseMgr::Worker::run()
{
	if (_mgr->_closing)
	{
		_running = false;
		return true;
	}
	AsyncQuery query;
	if (_mgr->_query_queue.try_pop(query))
	{
		while (!_mgr->_closing)
		{
			if (query._func)
			{
				query._func(_mgr->_db->query(query._sql));
				break;
			}

			DBConn* con = _mgr->_db->get_free_conn();
			if (!con)
			{
				continue;
			}
			_mgr->_db->send_query(con, query._sql.c_str());
			con->_lock.unlock();
			break;
		}
	}

	return true;
}
void DatabaseMgr::Worker::on_close()
{
	_mgr->_worker_lock.lock();
	_mgr->_worker = NULL;
	if (_mgr->_query_queue.get_size() != 0)
	{
		LogError("DATABASE", "Worker quit but the jobs are not finished!!!!!!!!!!!!!!!!!");
	}
	_mgr->_worker_lock.unlock();
}

DatabaseMgr::DatabaseMgr() : _inited(false), _closing(false), _config(NULL), _db(NULL)
{
}
DatabaseMgr::~DatabaseMgr()
{
	if (_inited)
		shutdown();
}

void DatabaseMgr::init(ConfigFile* config)
{
	if (_inited)
		return;
	_config = config;

	DB_TYPE type = (DB_TYPE)_config->get_value<long>("DB_TYPE");

	DatabaseInfo info;
	info._host_ip = _config->get_value<string>("DB_SERVER_IP");
	info._host_port = _config->get_value<unsigned int>("DB_SERVER_PORT");
	info._username = _config->get_value<string>("DB_USER_NAME");
	info._password = _config->get_value<string>("DB_PASSWORD");
	info._database = _config->get_value<string>("DB_DATABASE");

	switch (type)
	{
		case DB_MYSQL:
			_db = new MySQLDatabase();
			break;
		default:
			LogError("DATABASE", "Unknown database type has been found");
			break;
	}
	if (!_db)
	{
		LogError("Database", "Could not create database connector");
		return;
	}
	if (!_db->open(info))
	{
		delete _db;
		_db = NULL;
		LogError("Database", "Could not connect to database");
	}

	LogSuccess("Database", "Connectted to database successfully!!");
	_inited = true;
}

bool DatabaseMgr::is_inited()
{
	return _inited;
}

QueryResult* DatabaseMgr::query(const char* sql, ...)
{
	if (!_inited || !_db)
		return NULL;

	char sql_str[MAX_SQL_STRING_SIZE];
	memset(sql_str, 0, MAX_SQL_STRING_SIZE);
	va_list ap;
	va_start(ap, sql);
	vsnprintf(sql_str, MAX_SQL_STRING_SIZE, sql, ap);
	va_end(ap);

	return this->query(std::string(sql_str));
}
QueryResult* DatabaseMgr::query(const std::string& sql)
{
	if (!_inited || !_db)
		return NULL;
	return _db->query(sql);
}
bool DatabaseMgr::execute(const char* sql, ...)
{
	if (!_inited || !_db)
		return NULL;

	char sql_str[MAX_SQL_STRING_SIZE];
	memset(sql_str, 0, MAX_SQL_STRING_SIZE);
	va_list ap;
	va_start(ap, sql);
	vsnprintf(sql_str, MAX_SQL_STRING_SIZE, sql, ap);
	va_end(ap);
	this->execute(std::string(sql_str));
}
bool DatabaseMgr::execute(const std::string& sql)
{
	if (!_inited || !_db)
		return false;
	AsyncQuery eq;
	eq._sql = sql;
	eq._func = NULL;
	_LogDebug_("DBMGR", "EXECUTE SQL: %s", sql.c_str());
	_query_queue.push(eq);
	this->start_worker();
}
void DatabaseMgr::shutdown()
{
	_closing = true;
	_inited = false;

	// Wait for worker thread to finish its job.
	while (_worker)
	{
		usleep(100000);
	}
	if (0 != _query_queue.get_size())
	{
		LogError("DATABASE", "There are left qeurys have not processed!!!  Size: %d", _query_queue.get_size());
	}
	// Assertion for left jobs???
	//
	_query_queue.clear();
	if (_db)
	{
		_db->close();
		delete _db;
	}
	_closing = false;
}

void DatabaseMgr::start_worker()
{
	AutoLock lock(_worker_lock);
	if (_worker)
		return;
	_worker = new Worker(this);
	ThreadPool::get_singleton().execute(_worker);
}
