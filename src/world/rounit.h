#ifndef RO_UNIT_H_
#define RO_UNIT_H_

#include "unit.h"
#include "rocharinfo.h"
#include "mutex.h"

using ro::RoPosition;
using ro::RoCharInfoBase;

namespace srdgame
{
class Map;
class Packet;
class Player;
enum UnitType
{
	UT_SPAWNER,
	UT_MOB,
	UT_NPC,
	UT_PLAYER,
	UT_GM,
	UT_COUNT,
};
class RoUnit : public Unit
{
public:
	RoUnit(int id, UnitType type);
	// Will be deleted by who?  At last UnitMgr will delete all in its destructor.
	virtual ~RoUnit();

public:
	// Notified, return next needed notify time.  Normally this will be checked just as prefer not what will exactly happen.
	virtual long notify(long time);
	virtual NPriority get_priority()
	{
		return _pri;
	}

	virtual bool move_to(Position* pos, Object* by = NULL);
	virtual bool clicked(Object* by);
	virtual bool attacked(Object* by);

	virtual Position* get_pos()
	{
		return &_pos;
	}
	inline void set_map(Map* map)
	{
		_map = map;
	}
	inline Map* get_map()
	{
		return _map;
	}
	inline void set_info(RoCharInfoBase* info)
	{
		_info = info;	
	}

	inline UnitType get_type(RoUnit* unit)
	{
		// TODO: return different type when unit is different.
		return _type;
	}

	void get_names(Player* p);

	void send_info(Player* p);

	void get_item_value(Player* p);// Check all the items player has, and provide the values of his/her items.
	void get_selling_list(Player* p);// Send out the items we are selling.
protected:
	Mutex _lock;
	UnitType _type;
	RoCharInfoBase* _info;
	RoPosition _pos;
	NPriority _pri;
	long _time;
	Object* _by;
	Map* _map;
};
}

#endif
