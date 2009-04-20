#ifndef RO_UNIT_H_
#define RO_UNIT_H_

#include "unit.h"
#include "rocharinfo.h"

using ro::RoPosition;
using ro::RoCharInfoBase;

namespace srdgame
{
class Map;
class RoUnit : public Unit
{
public:
	RoUnit(int id);
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
	virtual RoCharInfoBase* get_info() = 0;
	inline void set_map(Map* map)
	{
		_map = map;
	}
	inline Map* get_map()
	{
		return _map;
	}

	virtual send_packet(const Packet* p) = 0;
protected:
	RoPosition _pos;
	NPriority _pri;
	long _time;
	Object* _by;
	Map* _map;
};
}

#endif
