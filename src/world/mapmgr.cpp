#include "mapmgr.h"
#include "map.h"
#include "intersocket.h"
#include "autolock.h"
#include "typedefs.h"
#include "mapconf.h"
#include "romap.h"
#include "rocharinfo.h"

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
	unload_maps();
}

void MapMgr::load_maps()
{
	unload_maps();
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
		_maps_data.resize(map_count);
		for (int i = 0; i < (int)map_count; ++i)
		{
			MapData data;
			int index = romap.get_map(maps[i].c_str(), data);
			if (-1 == index)
			{
				LogError(LN, "Could not load map : %s ", data._name);
				continue;
			}
			_maps_data.push_back(data);
			Map* new_map = new Map(maps[i]);
			new_map->bind(&data);
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
		usleep(10000);
		if (!_inter_socket->is_connected())
			break;
		_inter_socket->send_packet(&p);
	}
	return;
}
Map* MapMgr::get_map(std::string& name)
{
	return get_map(_map_ids[name]);
}
Map* MapMgr::get_map(int index)
{
	return _maps[index];
}
void MapMgr::unload_maps()
{
	AutoLock lock(_lock);
	_map_ids.clear();
	std::map<int, Map*>::iterator ptr = _maps.begin();
	for (; ptr != _maps.end(); ++ptr)
	{
		Map* m = ptr->second;
		delete m;
	}
	_maps.clear();
	size_t i = 0;
	for (; i < _maps_data.size(); ++i)
	{
		delete [] _maps_data[i]._cells;
	}
	_maps_data.clear();

}
