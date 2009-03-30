#include "player.h"
#include "worldsocket.h"
#include "map.h"
#include "charinfo.h"
#include "rocharinfo.h"

using namespace srdgame;
using namespace ro;
using namespace std;

Player::Player(WorldSocket* socket, int acc_id, int id) : _socket(socket), _acc_id(acc_id), _id(id)
{
	// Load player.
}
Player::~Player()
{
}

CharInfo* Player::get_info()
{
	return _info;
}

string Player::get_last_map()
{
	(string)((RoCharInfo*)_info)->_last_pos._map_name;
}

bool Player::set_map(Map* map)
{
	_map = map;
}

int Player::get_id()
{
	return _id;
}
int Player::get_acc_id()
{
	return _acc_id;
}

WorldSocket* Player::get_socket()
{
	return _socket;
}
