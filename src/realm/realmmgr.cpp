

#include "realmmgr.h"
#include "realmsocket.h"

#include "log.h"
#include "autolock.h"

using namespace srdgame;

RealmMgr::RealmMgr()
{
}
RealmMgr::~RealmMgr()
{
}
void RealmMgr::enum_world_servers(std::vector<WorldSrvInfo>& info)
{
	AutoLock lock(_server_lock);
	std::map<RealmSocketBase*, WorldSrvInfo>::iterator ptr = _servers_info.begin();
	for (; ptr != _servers_info.end(); ++ptr)
	{
		info.push_back(ptr->second);
	}
}
void RealmMgr::add_world_server(RealmSocketBase* s)
{
	AutoLock lock(_server_lock);
	RealmSocketBase* org = _servers[s->get_fd()];
	if (org)
	{
		LogError("RealmServer", "Two conflict server are registed, id : %d", s->get_fd());
		//org->close();
		// TODO:
	}
	_servers[s->get_fd()] = s;
}
void RealmMgr::remove_world_server(RealmSocketBase* s)
{
	AutoLock lock(_server_lock);
	//s->close();
	_servers.erase(s->get_fd());
	_servers_info.erase(s);
	std::map<RealmSocketBase*, std::vector<std::string> >::iterator ptr = _server_maps.find(s);
	if (ptr != _server_maps.end())
	{
		for (size_t i = 0; i < ptr->second.size(); ++i)
		{
			_map_servers.erase(ptr->second[i]);
		}
		_server_maps.erase(ptr);
	}
}

void RealmMgr::update_world_server(RealmSocketBase* s, WorldSrvInfo& info)
{
	AutoLock lock(_server_lock);
	_servers_info[s] = info;
}

void RealmMgr::update_world_server_name(RealmSocketBase* s, std::string name)
{
	AutoLock lock(_server_lock);
	::memcpy(_servers_info[s].name, name.c_str(), name.size());
}

void RealmMgr::update_world_server_status(RealmSocketBase* s, WorldSrvStatus status)
{
	AutoLock lock(_server_lock);
	_servers_info[s].status = status;
}

void RealmMgr::add_map(RealmSocketBase* s, WorldMapInfo* info)
{
	AutoLock lock(_server_lock);
	_server_maps[s].push_back(info->_name);
	_map_servers[info->_name] = s;
}
void RealmMgr::enum_maps(vector<string>& maps)
{
	AutoLock lock(_server_lock);
	map<RealmSocketBase*, vector<string> >::iterator ptr = _server_maps.begin();
	for (; ptr != _server_maps.end(); ++ptr)
	{
		copy(ptr->second.begin(), ptr->second.end(), back_inserter(maps));  
	}
}

void RealmMgr::add_client(RealmSocketBase* s)
{
	AutoLock lock(_client_lock);
	RealmSocketBase* org = _clients[s->get_fd()];
	if (org && org->is_connected())
	{
		LogError("RealmServer", "Two conflict clients are registed, id : %d", s->get_fd());
		org->close();
		// TODO:
	}
	_clients[s->get_fd()] = s;
}
void RealmMgr::remove_client(RealmSocketBase* s)
{
	AutoLock lock(_client_lock);
//	s->close();
	_clients.erase(s->get_fd());
}

void RealmMgr::set_name(std::string name)
{
	AutoLock lock(_lock);
	::memcpy(_info.name, name.c_str(), name.size());
}
std::string RealmMgr::get_name()
{
	AutoLock lock(_lock);
	return this->_info.name;
}

void RealmMgr::set_info(RealmSrvInfo& info)
{
	AutoLock lock(_lock);
	this->_info = info;
}
RealmSrvInfo& RealmMgr::get_info()
{	
	AutoLock lock(_lock);
	return this->_info;
}

bool RealmMgr::get_server_by_map(const std::string& map_name, WorldSrvInfo& info)
{
	RealmSocketBase* socket = _map_servers[map_name];
	if (!socket)
	{
		LogDebug("RealmServer", "No server for map : %s and map_server size : %d", map_name.c_str(), _map_servers.size());
		return false;
	}

	info = _servers_info[socket];
}
