#include "map.h"
#include "autolock.h"
#include "player.h"
#include "npc.h"
#include "log.h"

using namespace srdgame;
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

Map::Map(const string& name) : _name (name), _ro_map(NULL)
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
