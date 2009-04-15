#ifndef S_UNIT_H_
#define S_UNIT_H_

// The purpose of this unit is the base objects which will be hold by UnitContainer.
// Any Object in Game should be one unit.  Like one player's object, npc object, mob object, even other objects.

#include "waitor.h"
#include "object.h"


namespace srdgame
{
class Unit : public Waitor, public Object
{
public:
	Unit(int id) : _id (id) {}
	virtual ~Unit(){}

	int get_id() {return _id;}

private:
	int _id;
};
}

#endif
