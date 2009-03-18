#ifndef DATABASE_MANAGER_H_
#define DATABASE_MANAGER_H_

#include "singleton.h"
#include <string>
#include "threadbase.h"
#include "fastqueue.h"

namespace srdgame
{
class Database;
class ConfigFile;
class QueryResult;
class ConfigFile;

enum DB_TYPE
{
	DB_MYSQL = 0,
};
struct AsyncQuery
{
	std::string _sql;
	void (*_func)(QueryResult* res);
};

class DatabaseMgr : public Singleton < DatabaseMgr >
{
	class Worker : public ThreadBase
	{
	public:
		Worker(DatabaseMgr* mgr);
		// blocking function, return true means this thread finished to run, 
		// return false means continue to call this run function
		virtual bool run();
		// call to shutdown this thread.
		virtual void shutdown()
		{
			_running = false;
		}
		// call when this get closing? not work yet?
		virtual void on_close();
		// call to checking whether it is running.
		virtual bool is_running()
		{
			return _running;
		}
	private:
		bool _running;
		DatabaseMgr* _mgr;
	};
public:
	DatabaseMgr();
	~DatabaseMgr();
	void init(ConfigFile *config);
	bool is_inited();

	// Wrapper to the database.
	QueryResult* query(const char* sql, ...);
	QueryResult* query(const std::string& sql);
	bool execute(const char* sql, ...);
	bool execute(const std::string& sql);

	void shutdown();
protected:
	void start_worker();

private:
	bool _inited;
	bool _closing;
	ConfigFile* _config;
	Database* _db;
	FastQueue<AsyncQuery> _query_queue;

	Mutex _worker_lock;
	Worker* _worker;
};
}

#endif
