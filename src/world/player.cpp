#include "player.h"
#include "worldsocket.h"
#include "map.h"
#include "charinfo.h"
#include "rocharinfo.h"
#include "ro_defs.h"
#include "opcode.h"
#include "timedefs.h"
#include "gmsg.h"
#include "itemdefs.h"
#include "itemhelper.h"

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
void Player::add_item(RoCharItem& item)
{
	_lock.lock();
	RoCharInfo * info = (RoCharInfo*)_info;
	// TODO: check the count.
	info->_items.push_back(item);
	_lock.unlock();
	
	RoItemData data;
	if (item._type == 0)
		data._fail = 1;

	data._index = info->_items.size() - 1;
	data._id = item._type;
	data._identify = item._identify;
	data._equiped_point = item._equip;
	data._attrs = item._attrs;
	data._refine = item._refine;
	const RoDBItem* db = RoItemDB::get_singleton().get_item(item._type);
	data._equip_point = db->_equip_point;
	data._type = item._type;
	data._amount = item._amount;
	
	Packet p(ES_ADD_ITEM);
	p.len = sizeof(Packet) + sizeof(data);
	p.param.Data = (char*) &data;

	send_packet(&p);
}
void Player::update_look()
{
	// TODO:
}
void Player::update_look(RoLookType type, int val)
{
	RoLookUpdate update;
	update._type = type;
	update._account_id = _acc_id;//_id;
	update._id1 = val;
	update._id2 = 0; // TODO:
	Packet p(ES_LOOK_UPDATE);
	p.len = sizeof(Packet) + sizeof(update);
	p.param.Data = (char*) & update;
	send_packet(&p);
};
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
		// TODO: for pet egg.
		const RoDBItem* db = RoItemDB::get_singleton().get_item(ri->_items[i]._type);
		RoItemListData data;
		//data._info = &(ri->_items[i]);
		data._id = ri->_items[i]._type;
		data._type = db->_item_type;
		data._identify = ri->_items[i]._identify;
		data._equiped_point = ri->_items[i]._equip;
		data._attrs = ri->_items[i]._attrs;
		data._refine = ri->_items[i]._refine;
		data._amount = ri->_items[i]._amount;
		if (!isstackable2(db))
		{
			// Non-stackable (Equippable)

			data._equip_point = equippoint(db);

			LogDebug("Player", "One equippable item added");
			list.push_back(data);
		}
		else
		{
			// stackable.
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

void Player::equip_item(short index, short pos)
{
	RoCharInfo* info = (RoCharInfo*)_info;
	
	RoEquipItemOK ok;

	const RoDBItem* db = RoItemDB::get_singleton().get_item(info->_items[index]._type);
	if (index < info->_items.size() && db)
	{
		ok._index = index;
		ok._pos = equippoint(db);
		info->_items[index]._equip = ok._pos; // change
		ok._ok = 1;
	}
	else
	{
		ok._index = 0;
		ok._pos = 0;
		ok._ok = 0;
	}
	Packet packet(ES_EQUIP_ITEM);
	packet.len = sizeof(Packet) + sizeof(ok);
	packet.param.Data = (char*)&ok;
	send_packet(&packet);

	// Update look.
	if (ok._ok)
	{
		// Update look
		if (ok._pos & EQP_HAND_R)
		{
			info->_show._weapon = db->_look;
			update_look(LOOK_WEAPON, info->_show._weapon);
		}
		if (ok._pos & EQP_HEAD_LOW)
		{
			info->_show._head_bottom = db->_look;
			update_look(LOOK_HEAD_BOTTOM, info->_show._head_bottom);
		}
		if (ok._pos & EQP_HEAD_MID)
		{
			info->_show._head_middle = db->_look;
			update_look(LOOK_HEAD_MID, db->_look);
		}
		if (ok._pos & EQP_HEAD_TOP)
		{
			info->_show._head_top = db->_look;
			update_look(LOOK_HEAD_TOP, db->_look);
		}
		if (ok._pos & EQP_HAND_L)
		{
			info->_show._shield = db->_look;
			update_look(LOOK_SHIELD, db->_look);
		}
		// TODO: MOre looks
	}
	// TODO: Update status.
}

void Player::unequip_item(short index)
{
	RoCharInfo* info = (RoCharInfo*)_info;
	
	RoEquipItemOK ok;

	const RoDBItem* db = RoItemDB::get_singleton().get_item(info->_items[index]._type);
	if (index < info->_items.size() && db)
	{
		ok._index = index;
		ok._pos = equippoint(db);
		info->_items[index]._equip = ok._pos; // change
		ok._ok = 1;
	}
	else
	{
		ok._index = 0;
		ok._pos = 0;
		ok._ok = 0;
	}
	Packet packet(ES_UNEQUIP_ITEM);
	packet.len = sizeof(Packet) + sizeof(ok);
	packet.param.Data = (char*)&ok;
	send_packet(&packet);

	// Update look.
	if (ok._ok)
	{
		// Update look
		if (ok._pos & EQP_HAND_R)
		{
			info->_show._weapon = 0;
			update_look(LOOK_WEAPON, info->_show._weapon);
		}
		if (ok._pos & EQP_HEAD_LOW)
		{
			info->_show._head_bottom = 0;
			update_look(LOOK_HEAD_BOTTOM, 0);
		}
		if (ok._pos & EQP_HEAD_MID)
		{
			info->_show._head_middle = 0;
			update_look(LOOK_HEAD_MID, 0);
		}
		if (ok._pos & EQP_HEAD_TOP)
		{
			info->_show._head_top = 0;
			update_look(LOOK_HEAD_TOP, 0);
		}
		if (ok._pos & EQP_HAND_L)
		{
			info->_show._shield = 0;
			update_look(LOOK_SHIELD, 0);
		}
		// TODO: MOre looks

	}
	// TODO: Update status.
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
					ch = ch + i + 1;
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
		case EC_EQUIP_ITEM:
			{
				RoEquipItemUnion item;
				item._long = p->param.Long;
				equip_item(item._item._index, item._item._pos);
			}
			break;
		case EC_UNEQUIP_ITEM:
			{
				unequip_item(p->param.Int);
			}
			break;
		default:
			break;
	}
}

