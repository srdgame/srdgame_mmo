#include "map.h"
#include "autolock.h"
#include "player.h"
#include "npc.h"
#include "log.h"
#include "packetdefs.h"
#include "opcode.h"
#include "npcid.h"
#include  "strlib.h"

using namespace srdgame;
using namespace srdgame::opcode;
using namespace std;
using namespace ro;

#ifndef SOCKET_DEBUG
#define SOCKET_DEBUG
#undef _LogDebug_
#endif

#ifdef SOCKET_DEBUG
#define _LogDebug_ LogDebug
#else
#define _LogDebug_ //
#endif

#define LN "MAP"

Map::Map(const string& name, int id) : _name (name), _id(id), _ro_map(NULL)
{
}
Map::~Map()
{
	// Do not delete anything in mapdata, that will be free by mapmgr
}

void Map::bind(MapData* data)
{
	AutoLock lock(_lock);
	_ro_map = data;
}

bool Map::add_player(Player* p)
{
	AutoLock lock(_players_lock);
	if (_players.find(p->get_id()) != _players.end())
	{
		_LogDebug_(LN, "Player already existing on this map, id : %d", p->get_id());
		return false;
	}
	if (!p->set_map(this))
	{
		_LogDebug_(LN, "Player could not bind to this map, player id : %d", p->get_id());
		return false;
	}
	// Enumrate all units
	send_all_units(p);

	// Tell other players about new one.
	Packet packet(ES_NEW_PLAYER);
	packet.len = sizeof(Packet) + sizeof(RoCharInfoBase);
	packet.param.Data = (char*)p->get_info();

	send_to_all_players(&packet, p->get_id(), true);

	_players[p->get_id()] = p;
	return true;
}

bool Map::remove_player(Player* p)
{
	AutoLock lock(_players_lock);
	if (_players.end() == _players.find(p->get_id()))
	{
		_LogDebug_(LN, "Player is not in this map, map name : %s, player id: %d", _name.c_str(), p->get_id());
		return false;
	}
	_players.erase(p->get_id());
	return true;
}
void Map::add_unit(RoUnit* unit)
{
	AutoLock lock(_lock);
	//LogDebug("MAP", "Adding unit to map : %s", _name.c_str());
	unit->set_map(this);
	_mgr.add(unit);
}
void Map::remove_unit(RoUnit* unit)
{
	AutoLock lock(_lock);
	unit->set_map(NULL);
	_mgr.remove(unit);
}
void Map::send_msg(const string& msg, int from_id)
{
	AutoLock lock(_players_lock);
	std::map<int, Player*>::iterator ptr = _players.find(from_id);
	if (ptr == _players.end())
		return;
	Packet p(ES_MESSAGE);
	p.len = sizeof(Packet) + sizeof(RoMessage);
	RoMessage m;
	m._id = from_id; // Currently the player id is the char_id;
	//char buf[4096];
	//StringToWChar(buf, msg, 4096);
	m._msg = msg.c_str();
	m._len = msg.length();
	p.param.Data = (char*)&m;
	send_to_all_players(&p, from_id, true);
}
void Map::request_char_name(Player* p, int id)
{
	AutoLock lock(_lock);
	if (NpcId::is_npc(id))
	{
		RoUnit* unit = _mgr.get_unit(id);
		if (!unit)
			return;

		unit->get_names(p); // Let unit say its names.
	}
	else
	{
		if (_players.find(id) != _players.end())
		{
			Player* dp = _players[id];
			if (dp)
			{
				// TODO:
			}
		}
	}
}
void Map::click_npc(Player* p, int id)
{
	char buf[4096];
	if (NpcId::is_npc(id))
	{
		AutoLock lock(_lock);
		RoUnit* unit = _mgr.get_unit(id);
		if (!unit)
			return;
		// TODO:
		if (!unit->clicked(p->get_unit()))
			return;
		switch (unit->get_type(p->get_unit()))
		{
			case UT_NPC:
				{
					Packet bs(ES_CLICK_NPC_BUY_SELL);
					bs.param.Int = id;
					p->send_packet(&bs);
				}
				break;
			case UT_MOB:
				{
					// TODO:
				}
				break;
			default:
				break;
		}
	}
	else
	{
		// TODO:
	}
}
void Map::request_buy_sell_list(int npc_id, int flag, Player* p)
{
	if (!NpcId::is_npc(npc_id))
		return;

	AutoLock lock(_lock);
	RoUnit* unit = _mgr.get_unit(npc_id);
	if (!unit)
		return;
	if (flag == 0)
	{
		// ask for npc's sell items.
		unit->get_selling_list(p);
	}
	else
	{
		// sell to npc about items.
		unit->get_item_value(p);
	}
}
void Map::send_to_all(Packet* p, int from_id, bool skip_self)
{
	AutoLock lock(_lock);
	send_to_all_players(p, from_id, skip_self);
}
void Map::send_to_all_players(Packet* p, int from_id, bool skip_self)
{
	// Send to all user.
	std::map<int, Player*>::iterator ptr = _players.begin();
	for (; ptr != _players.end(); ++ptr)
	{
		// Do not send back to the user 	who is sending out message.
		if (skip_self && ptr->second->get_id() == from_id)
			continue;

		ptr->second->send_packet(p);
	}
}
/*void Map::send_to_unit(Packet* p, int unit_id)
{
	AutoLock lock(_lock);
	RoUnit* unit = _mgr.get_unit(unit_id);
	if (unit)
	{
		unit->send_packet(p);
	}
}*/

void Map::send_all_units(Player* p)
{
	AutoLock lock(_lock);

	vector<RoUnit*> units;
	_mgr.get_all(units);

	LogError("MAP", "Map: %s currenttly has unit : %u", _name.c_str(), units.size());
	for (size_t i = 0; i < units.size(); ++i)
	{
		if (units[i] == NULL)
			continue;
		units[i]->send_info(p);
	}
}
