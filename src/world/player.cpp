#include "player.h"
#include "worldsocket.h"
#include "map.h"
#include "charinfo.h"
#include "rocharinfo.h"
#include "ro_defs.h"
#include "opcode.h"
#include "timedefs.h"
#include "gmsg.h"

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
	LogError("Player", "Set_info~~~~~~~~~~~~~~~");
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

	// send items
	this->send_items();
}

CharInfo* Player::get_info() const
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

int Player::get_id() const
{
	return _id;
}
int Player::get_acc_id() const
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
int isequip2(RoCharItem& data)
{ 
	switch(data._item_type) {
		case IT_WEAPON:
		case IT_ARMOR:
		case IT_AMMO:
			return 1;
		default:
			return 0;
	}
}
int isstackable2(RoCharItem& item)
{
  switch(item._item_type) {
	  case IT_WEAPON:
	  case IT_ARMOR:
	  case IT_PETEGG:
	  case IT_PETARMOR:
		  return 0;
	  default:
		  return 1;
  }
}
int equippoint(RoCharItem& item)
{
	int ep = 0;

	if (!isequip2(item))
		return 0; //Not equippable by players.
	
	ep = item._equip;
	// TODO:
	/*if(sd->inventory_data[n]->look == W_DAGGER	||
		sd->inventory_data[n]->look == W_1HSWORD ||
		sd->inventory_data[n]->look == W_1HAXE) {
		if(ep == EQP_HAND_R && (pc_checkskill(sd,AS_LEFT) > 0 || (sd->class_&MAPID_UPPERMASK) == MAPID_ASSASSIN))
			return EQP_ARMS;
	}*/
	return ep;
}
void Player::send_items()
{
	LogError("Player", "Send items !!!!!!!!!!!!!!!!");
	RoCharInfo* ri = (RoCharInfo*)_info;
	if (ri->_items.size() == 0)
		return;

	vector<RoItemListData> list; // equipable?
	vector<RoItemListData> list2; 
	for (size_t i = 0; i < ri->_items.size(); ++i)
	{
		if (!isstackable2(ri->_items[i]))
		{
			// Non-stackable (Equippable)
			RoItemListData data;
			data._info = &(ri->_items[i]);
			data._equip_point = equippoint(ri->_items[i]);
			LogDebug("Player", "One equippable item added");
			list.push_back(data);
		}
		else
		{
			// stackable.
			RoItemListData data;
			data._info = &(ri->_items[i]);
			data._equip_point = -2;
			LogDebug("Player", "One stackable item added");
			list.push_back(data);
		}
	}
	RoItemList rl;
	if (list.size())
	{
		rl._items = &list;
		Packet p(ES_EQUIP_ITEM_LIST);
		p.len = sizeof(Packet) + sizeof(rl);
		p.param.Data = (char*) &rl;
		send_packet(&p);
	}
	if (list2.size())
	{
		rl._items = &list2;
		Packet p(ES_STACKABLE_ITEM_LIST);
		p.len = sizeof(Packet) + sizeof(rl);
		p.param.Data = (char*) &rl;
		send_packet(&p);
	}
}

void Player::on_handle(Packet* p)
{
	LogDebug("Player", "On Player::on_handle, op : %d", p->op);
	RoCharInfo* info = (RoCharInfo*)_info;
	switch (p->op)
	{
		case EC_MESSAGE:
			{
				RoMessage m;
				m._len = p->len - sizeof(Packet);
				m._msg = p->param.Data;
				m._id = 0;
				Packet msg(ES_MESSAGE);
				msg.len = sizeof(Packet) + sizeof(RoMessage);
				msg.param.Data = (char*)&m;
				send_packet(&msg); // send back the message to show it in GUI.

				// Send to other players.
				const char* ch = m._msg;
				int i = 0;
				while (*(ch + i) != ':' && i < m._len)
				{
					++i;
				}
				if (i != m._len)
				{
					ch = ch + i;
					if (_map)
					{
						GMsg::get_singleton().send(_map->get_id(), ch, this);
					}
				}
			}
			break;
		case EC_TICK_COUNT:
			{
				Packet tick(ES_TICK_COUNT);
				tick.param.Int = p->param.Int;
				send_packet(&tick);
			}
			break;
		case EC_WALK_TO:
			{
				Packet walk(ES_WALK_TO);
				RoWalkToXY xy;
				xy._int = p->param.Int;
				LogError("Player", "Want to walk to x : %d, y : %d", xy._point._x, xy._point._y);
				walk.len = sizeof(Packet) + sizeof(RoWalkToXY_OK);
				RoWalkToXY_OK ok;
				ok._tick = gettick();
				ok._org._x = info->_last_pos._x;
				ok._org._y = info->_last_pos._y;
				ok._to = xy._point;
				LogError("Player", "Let it walk from x: %d, y : %d", ok._org._x, ok._org._y);

				// save to last_pos.
				info->_last_pos._x = xy._point._x;
				info->_last_pos._y = xy._point._y;

				walk.param.Data = (char*) &ok;

				send_packet(&walk);

				/*RoUnitMove move;
				move._tick = ok._tick;
				move._org = ok._org;
				move._to = ok._to;
				move._id = info->_id;
				
				Packet mp(ES_UNIT_MOVE);
				mp.len = sizeof(Packet) + sizeof(RoUnitMove);
				mp.param.Data = (char*)&move;
				
				send_packet(&mp);*/
			}
			break;
		default:
			break;
	}
}

