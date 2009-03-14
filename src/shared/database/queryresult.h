#ifndef QUERY_RESULT_H_
#define QUERY_RESULT_H_

#include "threadbase.h"
#include "field.h"

namespace srdgame
{
class QueryResult
{
public:
	QueryResult(size_t field_size, size_t row_size): _field_size(field_size), _row_size(row_size), _cur(NULL)
	{
		_cur = new Field[field_size];
	}
	virtual ~QueryResult()
	{
		delete[] _cur;
	}

	virtual bool next() = 0;
	
	inline void Delete()
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
protected:
	size_t _field_size;
	size_t _row_size;
	Field* _cur;
};
}

#endif
