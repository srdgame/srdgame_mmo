#ifndef RO_UNIT_H_
#define RO_UNIT_H_

#include "unit.h"
#include "rocharinfo.h"

using ro::RoPosition;

namespace srdgame
{
class RoUnit : public Unit
{
public:
	RoUnit(int id);
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

private:
	RoPosition _pos;
	NPriority _pri;
	long _time;
	Object* _by;
};
}

#endif
