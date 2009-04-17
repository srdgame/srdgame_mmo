#ifndef S_MOB_H_
#define S_MOB_H_

#include "npc.h"

namespace srdgame
{
class Spawner;
class Mob : public Npc
{
	friend class MobObject;
public:
	Mob(Spawner* sp);
	// will be deleted by Spawner.
	virtual ~Mob();
};
}

#endif
