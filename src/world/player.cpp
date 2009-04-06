#include "player.h"
#include "worldsocket.h"
#include "map.h"
#include "charinfo.h"
#include "rocharinfo.h"
#include "opcode.h"

using namespace srdgame;
using namespace srdgame::opcode;
using namespace ro;
using namespace std;

Player::Player(int acc_id, int id) : _socket(NULL), _acc_id(acc_id), _id(id), _info(NULL)
{
	// Load player.
}
Player::~Player()
{
	_lock.lock();
	_socket->bind(NULL);
	_socket->close();
	delete _info;
	_info = NULL;
	_lock.unlock();
}

void Player::set_info(CharInfo* info)
{
	if (info->_id != _id)
	{
		LogError("Player", "Seting up incorrect infomation for player. info->id : %d, _id : %d", info->_id, _id);
		return;
	}

	_lock.lock();
	delete _info;
	_info = info;
	_lock.unlock();

	// Send friend list.
	this->send_friend_list();
}

CharInfo* Player::get_info()
{
	return _info;
}

string Player::get_last_map()
{
	if (!_info)
		return "";
	_lock.lock();
	RoCharInfo* ri = (RoCharInfo*)_info;
	LogDebug("Player", "The player last map is %s", ri->_last_pos._map_name);
 	string map(ri->_last_pos._map_name);
	_lock.unlock();
	return map;
}

bool Player::set_map(Map* map)
{
	_lock.lock();
	_map = map;
	_lock.unlock();
	return true;
}

int Player::get_id()
{
	return _id;
}
int Player::get_acc_id()
{
	return _acc_id;
}
void Player::bind(WorldSocket* socket)
{
	_lock.lock();
	_socket = socket;
	_lock.unlock();
}
void Player::send_packet(Packet* p)
{
	_lock.lock();
	if (_socket)
		_socket->send_packet(p);
	_lock.unlock();
}
void Player::update_look()
{
	// TODO:
}
void Player::send_friend_list()
{
	// TODO:
	RoCharInfo* ri = (RoCharInfo*)_info;
	Packet p;
	p.op = ES_FRIEND_LIST;
	p.len = sizeof(Packet) + sizeof(RoFriendList);
	RoFriendList list;
	list._friends = &(ri->_friends);
	p.param.Data = (char*)&list;
	send_packet(&p);
	// TODO:
	// send out online friends info.
}

void Player::on_handle(Packet* p)
{
	switch (p->op)
	{
		default:
			break;
	}
}

