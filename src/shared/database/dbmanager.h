#ifndef DATABASE_MANAGER_H_
#define DATABASE_MANAGER_H_

#include "singleton.h"

namespace srdgame
{
enum DB_TYPE
{
};
class Database;
class Config;
class DBManager : public Singleton < DBManager >
{
public:
	void init(Config conf, DB_TYPE type);
	bool is_inited();
	Database& DB()
	{
		return *_db;
	}
	void shutdown();

private:
	bool _inited;
	Database* _db;
};
}

#endif
