#ifndef RO_ITEM_DB_H_
#define RO_ITEM_DB_H_

#include "itemdefs.h"
#include "singleton.h"
#include <cassert>

namespace srdgame
{
struct Field;
};
namespace ro
{
class RoItemDB : public srdgame::Singleton < RoItemDB >
{
public:
	RoItemDB() : _inited(false), _max_id(0), _items(NULL)
	{
	
	}
	~RoItemDB()
	{
		delete [] _items;
	}

	void init(short max_id= 4096);

	inline const RoDBItem* get_item(short id)
	{
		assert(_items != NULL);
		assert(_max_id > id);
		return _items + id;
	}
	inline short get_max_id()
	{
		return _max_id;
	}
protected:
	bool fetch_item(srdgame::Field* f);
private:
	bool _inited;
	short _max_id;
	RoDBItem* _items;
};
};

#endif
