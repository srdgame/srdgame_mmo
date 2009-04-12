#include "map.h"
#include "autolock.h"
#include "player.h"
#include "npc.h"
#include "log.h"
#include "packetdefs.h"
#include "opcode.h"

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

void Map::add_npc(Npc* npc)
{
	AutoLock lock(_npcs_lock);
	_npcs[npc->get_id()] = npc;
}
void Map::remove_npc(Npc* npc)
{
	AutoLock lock(_npcs_lock);
	_npcs.erase(npc->get_id());
}

void Map::send_msg(const string& msg, int pid)
{
	AutoLock lock(_players_lock);
	std::map<int, Player*>::iterator ptr = _players.find(pid);
	if (ptr == _players.end())
		return;
	Packet p(ES_MESSAGE);
	p.len = sizeof(Packet) + sizeof(RoMessage);
	RoMessage m;
	m._id = pid; // Currently the player id is the char_id;
	m._msg = msg.c_str();
	m._len = msg.length();
	p.param.Data = (char*)&m;

	// Send to all user.
	ptr == _players.begin();
	for (; ptr != _players.end(); ++ptr)
	{
		// Do not send back to the user who is sending out message.
		if (ptr->second->get_id() == pid)
			continue;

		ptr->second->send_packet(&p);
	}
}
