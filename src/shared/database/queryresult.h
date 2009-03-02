#ifndef QUERY_RESULT_H_
#define QUERY_RESULT_H_

#include "threadbase.h"

namespace srdgame
{
class Field;
class QueryResult
{
public:
	QueryResult(size_t field_size, size_t row_size): _field_size(field_size), _row_size(row_size), _cur(NULL)
	{
	}
	virtual ~QueryResult()
	{
	}

	virtual bool next() = 0;
	
	inline void delete_this()
	{
		delete this;
	}

	inline Field* fetch()
	{
		return _cur;
	}
	inline size_t get_field_size()
	{
		return _field_size;
	}
	inline size_t get_row_size()
	{
		return _row_size;
	}
private:
	size_t _field_size;
	size_t _row_size;
	Field* _cur;
};
class Database;
class QueryThread : public ThreadBase
{
public:
	QueryThread(Database* db);
	~QueryThread();

	// blocking function, return true means delete this threadobject.
	virtual bool run() = 0;
	// call to shutdown this thread.
	virtual void shutdown() = 0;
	// call when this get closing? not work yet?
	virtual void on_close() = 0;
	// call to checking whether it is running.
	virtual bool is_running() = 0;
};
}

#endif
