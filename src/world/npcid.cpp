#include "npcid.h"

using namespace srdgame;

int NpcId::_cur_id = MIN_NPC_ID;

/*int NpcId::generated()
{
	// We may need lock later, but so far this is should be fine.
	return ++_cur_id;
}*/
