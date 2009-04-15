#include "npc.h"
#include "npcid.h"

using namespace srdgame;

Npc::Npc()
{
	_id = NpcId::generate(); // generate the id;
}

Npc::~Npc()
{
}


