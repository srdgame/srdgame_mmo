#ifndef NPC_H_
#define NPC_H_

#include "rocharinfo.h"

using ro::RoCharInfoBase;

namespace srdgame
{
class Spawner;
class NpcObject;
class RoUnit;
class Npc
{
	friend class NpcObject;
public:
	Npc(Spawner* sp);

	// Will be deleted by Spawner.
	virtual ~Npc();
	int get_id()
	{
		return _id;
	}
protected:
	int _id;
	RoCharInfoBase _info;
	Spawner* _sp;
	RoUnit* _obj;
};
}

#endif
