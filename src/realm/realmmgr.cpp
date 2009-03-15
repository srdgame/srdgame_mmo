

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
	if (org && org->is_connected())
	{
		LogError("RealmServer", "Two conflict server are registed, id : %d", s->get_fd());
		org->close();
		// TODO:
	}
	_servers[s->get_fd()] = s;
}
void RealmMgr::remove_world_server(RealmSocketBase* s)
{
	AutoLock lock(_server_lock);
	s->close();
	_servers.erase(s->get_fd());
	_servers_info.erase(s);
}

void RealmMgr::update_world_server(RealmSocketBase* s, WorldSrvInfo& info)
{
	AutoLock lock(_server_lock);
	_servers_info[s] = info;
}

void RealmMgr::update_world_server_name(RealmSocketBase* s, std::string name)
{
	AutoLock lock(_server_lock);
	_servers_info[s].name = name;
}

void RealmMgr::update_world_server_info(RealmSocketBase* s, std::string info)
{
	AutoLock lock(_server_lock);
	_servers_info[s].info = info;
}

void RealmMgr::update_world_server_status(RealmSocketBase* s, WorldSrvStatus status)
{
	AutoLock lock(_server_lock);
	_servers_info[s].status = status;
}
void RealmMgr::update_world_server_type(RealmSocketBase* s, WorldSrvType type)
{
	AutoLock lock(_server_lock);
	_servers_info[s].type = type;
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
	s->close();
	_clients.erase(s->get_fd());
}

void RealmMgr::set_name(std::string name)
{
	AutoLock lock(_lock);
	this->_name = name;
}
std::string RealmMgr::get_name()
{
	AutoLock lock(_lock);
	return this->_name;
}

void RealmMgr::set_info(std::string info)
{
	AutoLock lock(_lock);
	this->_info = info;
}
std::string RealmMgr::get_info()
{	
	AutoLock lock(_lock);
	return this->_info;
}
