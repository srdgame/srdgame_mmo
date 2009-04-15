#include "playerobject.h"


using namespace srdgame;

PlayerObject::PlayerObject(Player& player) : RoUnit(player.get_id()), _player(player)
{
}

PlayerObject::~PlayerObject()
{
}
