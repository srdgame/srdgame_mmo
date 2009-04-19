#include "mobobject.h"

using namespace srdgame;

MobObject::MobObject(Mob& mob) : RoUnit(mob.get_id()), _mob(mob)
{
	_mob._obj = this;
}

MobObject::~MobObject()
{
	_mob._obj = NULL;
}