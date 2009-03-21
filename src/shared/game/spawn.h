#ifndef SPAWN_H_
#define SPAWN_H_
#include "nobject.h"

namespace srdgame
{
enum SpawnType
{
	ST_ITEM,
	ST_NPC,
	ST_SPAWN,
};
struct SpawnInfo
{
	int _id;
	int _spawn_obj_id;
	SpawnType _type;
};

class SpawnObject : public NObject
{
};
}

#endif
