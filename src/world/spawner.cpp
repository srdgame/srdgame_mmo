#include "spawner.h"

using namespace srdgame;

Spawner::Spawner(const SpawnerInfo& info, const RoPosition& pos) : _info(info)
{
	_pos = pos;//
}

Spawner::~Spawner()
{
}

