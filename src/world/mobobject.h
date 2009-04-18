#ifndef MOB_OBJECT_H_
#define MOB_OBJECT_H_

#include "rounit.h"
#include "mob.h"

namespace srdgame
{
class MobObject : public RoUnit
{
public:
	MobObject(Mob& mob);
	virtual ~MobObject(); // Will be deleted by Npc

	virtual RoCharInfoBase* get_info()
	{
		return &_mob._info;
	}
private:
	Mob _mob;
};

}

#endif
