

#include "loginmgr.h"
#include "loginsocket.h"

#include "log.h"
#include "autolock.h"

using namespace srdgame;

LoginMgr::LoginMgr()
{
}
LoginMgr::~LoginMgr()
{
}
void LoginMgr::enum_world_servers(std::vector<WorldSrvInfo>& info)
{
	AutoLock lock(_server_lock);
	std::map<LoginSocketBase*, WorldSrvInfo>::iterator ptr = _servers_info.begin();
	for (; ptr != _servers_info.end(); ++ptr)
	{
		info.push_back(ptr->second);
	}
}
void LoginMgr::add_world_server(LoginSocketBase* s)
{
	AutoLock lock(_server_lock);
	LoginSocketBase* org = _servers[s->get_fd()];
	if (org && org->is_connected())
	{
		LogError("LoginServer", "Two conflict server are registed, id : %d", s->get_fd());
		org->close();
		// TODO:
	}
	_servers[s->get_fd()] = s;
}
void LoginMgr::remove_world_server(LoginSocketBase* s)
{
	AutoLock lock(_server_lock);
	s->close();
	_servers.erase(s->get_fd());
	_servers_info.erase(s);
}

void LoginMgr::update_world_server(LoginSocketBase* s, WorldSrvInfo& info)
{
	AutoLock lock(_server_lock);
	_servers_info[s] = info;
}

void LoginMgr::update_world_server_name(LoginSocketBase* s, std::string name)
{
	AutoLock lock(_server_lock);
	_servers_info[s].name = name;
}

void LoginMgr::update_world_server_info(LoginSocketBase* s, std::string info)
{
	AutoLock lock(_server_lock);
	_servers_info[s].info = info;
}

void LoginMgr::update_world_server_status(LoginSocketBase* s, WorldSrvStatus status)
{
	AutoLock lock(_server_lock);
	_servers_info[s].status = status;
}
void LoginMgr::update_world_server_type(LoginSocketBase* s, WorldSrvType type)
{
	AutoLock lock(_server_lock);
	_servers_info[s].type = type;
}
void LoginMgr::add_client(LoginSocketBase* s)
{
	AutoLock lock(_client_lock);
	LoginSocketBase* org = _clients[s->get_fd()];
	if (org && org->is_connected())
	{
		LogError("LoginServer", "Two conflict clients are registed, id : %d", s->get_fd());
		org->close();
		// TODO:
	}
	_clients[s->get_fd()] = s;
}
void LoginMgr::remove_client(LoginSocketBase* s)
{
	AutoLock lock(_client_lock);
	s->close();
	_clients.erase(s->get_fd());
}

void LoginMgr::set_name(std::string name)
{
	AutoLock lock(_lock);
	this->_name = name;
}
std::string LoginMgr::get_name()
{
	AutoLock lock(_lock);
	return this->_name;
}

void LoginMgr::set_info(std::string info)
{
	AutoLock lock(_lock);
	this->_info = info;
}
std::string LoginMgr::get_info()
{	
	AutoLock lock(_lock);
	return this->_info;
}
