#include "npcid.h"

using namespace srdgame;

int NpcId::_cur_id = 100000000;

/*int NpcId::generated()
{
	// We may need lock later, but so far this is should be fine.
	return ++_cur_id;
}*/
