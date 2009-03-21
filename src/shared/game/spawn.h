#ifndef SPAWN_H_
#define SPAWN_H_
#include "nobject.h"

namespace srdgame
{
enum SpawnType
{
	ST_ITEM;
	ST_NPC;
	ST_SPAWN;
};
struct Spawn
{
	int _id;
	int _spawn_obj_id;
	Spawn _type;
};

class SpawnObject : public NObject
{
};
}

#endif
