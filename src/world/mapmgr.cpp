#include "mapmgr.h"
#include "map.h"
#include "intersocket.h"
#include "autolock.h"
#include "typedefs.h"

using namespace srdgame;
using namespace std;

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

MapMgr::MapMgr() : _inter_socket(NULL)
{
}
MapMgr::~MapMgr()
{
}

void MapMgr::load_maps()
{
	AutoLock lock(_lock);
	_maps[1] = new Map("AAAA");
	_map_ids["AAAA"] = 1;
}
void MapMgr::bind(InterSocket* socket)
{
	AutoLock lock(_lock);
	_inter_socket = socket;
}
void MapMgr::send_maps()
{
	AutoLock lock(_lock);
	if (!_inter_socket)
		return;
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
		_inter_socket->send_packet(&p);
	}
	return;
}
