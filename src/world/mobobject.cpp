#include "mobobject.h"

using namespace srdgame;

MobObject::MobObject(Mob& mob) : RoUnit(mob.get_id()), _mob(mob)
{
}

MobObject::~MobObject()
{
}
