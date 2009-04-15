#ifndef PLAYER_OBJECT_H_
#define PLAYER_OBJECT_H_

#include "rounit.h"
#include "player.h"

namespace srdgame
{
class PlayerObject : RoUnit
{
public:
	PlayerObject(Player& p);
	virtual ~PlayerObject();

private:
	Player& _player;
};
}

#endif
