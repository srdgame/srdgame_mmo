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

	inline int get_id()
	{
		return _info._id;
	}

protected:
	RoCharInfoBase _info;
	Spawner* _sp;
	RoUnit* _obj;
};
}

#endif
