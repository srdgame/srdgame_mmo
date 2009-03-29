#include "mapmgr.h"
#include "map.h"
#include "intersocket.h"
#include "autolock.h"
#include "typedefs.h"
#include "mapconf.h"
#include "romap.h"

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

#define LN "MapMgr"

#define MAP_CONF_FILE_NAME "./data/conf/maps_athena.conf"
#define RO_MAP_CACHE_FILE "./data/db/map_cache.dat"

MapMgr::MapMgr() : _inter_socket(NULL)
{
}
MapMgr::~MapMgr()
{
}

void MapMgr::load_maps()
{
	AutoLock lock(_lock);

	// Start to load romaps.
	RoMap romap;
	if (romap.load(RO_MAP_CACHE_FILE))
	{
	}
	/*
	_maps[1] = new Map("AAAA");
	_map_ids["AAAA"] = 1;*/

	// Load the configed map for this world server.
	MapConf mc(MAP_CONF_FILE_NAME);
	if(mc.load())
	{
		vector<string>& maps = mc.get_maps();
		size_t map_count = maps.size();
		for (int i = 0; i < (int)map_count; ++i)
		{
			Map* new_map = new Map(maps[i]);
			_maps.insert(pair<int, Map*>(i, new_map));
			_map_ids.insert(pair<string, int>(maps[i], i));
		}
		LogSuccess(LN, "Loaded map configuration successfuly, count : %d", map_count);
	}
	else
	{
		LogError(LN, "Could not load map configuration from file :%s", MAP_CONF_FILE_NAME);
	}

}
void MapMgr::bind(InterSocket* socket)
{
	AutoLock lock(_lock);
	_inter_socket = socket;
}
void MapMgr::send_maps()
{
	LogDebug("MAP_MGR", "Start to send maps");
	AutoLock lock(_lock);
	if (!_inter_socket)
		return;
	// Test for really big data.
	//
	/*char* d = new char[10000];
	::memset(d, 0, 10000);
	_inter_socket->send(d, 10000);

	return;*/

	// end of Test
	std::map<std::string, int>::iterator ptr = _map_ids.begin();

	WorldMapInfo info;
	Packet p;
	p.op = IC_MAPS;
	p.len = sizeof(Packet) + sizeof(WorldMapInfo);
	p.param.Data = (char*)&info;
	for (; ptr != _map_ids.end(); ++ptr)
	{
		info._index = ptr->second;
		memset(info._name, 0, 64);
		memcpy(info._name, ptr->first.c_str(), min((int)ptr->first.length(), 64));
		//usleep(10000);
		_inter_socket->send_packet(&p);
	}
	return;
}
