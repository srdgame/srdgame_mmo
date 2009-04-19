#include "map.h"
#include "autolock.h"
#include "player.h"
#include "npc.h"
#include "log.h"
#include "packetdefs.h"
#include "opcode.h"
#include "npcid.h"
#include "npcobject.h"
#include "mobobject.h"
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

	send_packet(&packet, p->get_id(), true);

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
	send_packet(&p, from_id, true);
}
void Map::request_char_name(Player* p, int id)
{
	char buf[4096];
	if (NpcId::is_npc(id))
	{
		RoUnit* unit = _mgr.get_unit(id);
		if (!unit)
			return;

		// TODO: test whether we could only use the ex name.
		if (dynamic_cast<NpcObject*>(unit))
		{
			// For npc name.
			Packet packet(ES_REQUEST_CHAR_NAME);
			RoRequestCharName name;
			memset(&name, 0, sizeof(name));
			name._id = id;
			//StringToWChar(name._name, unit->get_info()->_name, MAX_NAME_LEN);
			memcpy(name._name, unit->get_info()->_name.c_str(), MAX_NAME_LEN);
			packet.len = sizeof(Packet) + sizeof(name);
			packet.param.Data = (char*) &name;
			p->send_packet(&packet);
		}
		else
		{
			Packet packet(ES_REQUEST_CHAR_NAME_EX);
			RoRequestCharNameEx name;
			memset(&name, 0, sizeof(name));
			name._id = id;
			//StringToWChar(name._name, unit->get_info()->_name, MAX_NAME_LEN);
			memcpy(name._name, unit->get_info()->_name.c_str(), MAX_NAME_LEN);
			packet.len =sizeof(packet) + sizeof(name);
			packet.param.Data = (char*)&name;
			p->send_packet(&packet);
		}
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
void Map::send_packet(Packet* p, int from_id, bool skip_self)
{
	// Send to all user.
	std::map<int, Player*>::iterator ptr = _players.begin();
	for (; ptr != _players.end(); ++ptr)
	{
		// Do not send back to the user who is sending out message.
		if (skip_self && ptr->second->get_id() == from_id)
			continue;

		ptr->second->send_packet(p);
	}
}
void Map::send_all_units(Player* p)
{
	Packet packet(ES_MOB_INFO);
	packet.len = sizeof(Packet) + sizeof(RoCharInfoBase);
	vector<RoUnit*> units;
	_mgr.get_all(units);

	LogError("MAP", "Map: %s currenttly has unit : %u", _name.c_str(), units.size());
	for (size_t i = 0; i < units.size(); ++i)
	{
		if (units[i] == NULL)
			continue;
		if (units[i]->get_info() == NULL)
			continue;
		LogDebug("MAP", "Unit in X:%d \t Y:%d \t TYPE: %d", units[i]->get_info()->_last_pos._x, units[i]->get_info()->_last_pos._y, units[i]->get_info()->_show._class);
		packet.param.Data = (char*) units[i]->get_info();
		p->send_packet(&packet);
	}
}
