#ifndef NPC_H_
#define NPC_H_

#include "rocharinfo.h"

using ro::RoCharInfoBase;

namespace srdgame
{
class Npc
{
public:
	Npc();
	virtual ~Npc();
	int get_id()
	{
		return _id;
	}
protected:
	int _id;
	RoCharInfoBase _info;
};
}

#endif
