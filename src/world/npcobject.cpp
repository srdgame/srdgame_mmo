#include "npcobject.h"

using namespace srdgame;

NpcObject::NpcObject(Npc& npc) : RoUnit(npc.get_id()), _npc(npc)
{
	_npc._obj = this;
}

NpcObject::~NpcObject()
{
	_npc._obj = NULL;
}

long NpcObject::notify(long time)
{
	
	return RoUnit::notify(time);
}

bool NpcObject::move_to(Position* pos, Object* by)
{
	return RoUnit::move_to(pos, by);
}

bool NpcObject::clicked(Object* by)
{
	return RoUnit::clicked(by);
}

bool NpcObject::attacked(Object* by)
{
	return RoUnit::attacked(by);
}


