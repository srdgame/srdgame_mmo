#ifndef NPC_OBJECT_H_
#define NPC_OBJECT_H_

#include "rounit.h"
#include "npc.h"

namespace srdgame
{
class NpcObject : public RoUnit
{
public:
	NpcObject(Npc& npc);
	virtual ~NpcObject(); // will be deleted by Npc

public:
	// Notified, return next needed notify time.  Normally this will be checked just as prefer not what will exactly happen.
	virtual long notify(long time);

	virtual bool move_to(Position* pos, Object* by = NULL);
	virtual bool clicked(Object* by);
	virtual bool attacked(Object* by);

	virtual RoCharInfoBase* get_info()
	{
		return &_npc._info;
	}
private:
	Npc& _npc;
};
}

#endif
