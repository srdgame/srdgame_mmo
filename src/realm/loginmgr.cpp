

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
void LoginMgr::enum_realm_servers(std::vector<RealmSrvInfo>& info)
{
	AutoLock lock(_server_lock);
	std::map<LoginSocket*, RealmSrvInfo>::iterator ptr = _servers_info.begin();
	for (; ptr != _servers_info.end(); ++ptr)
	{
		info.push_back(ptr->second);
	}
}
void LoginMgr::add_realm_server(LoginSocket* s)
{
	LogDebug("LoginServer", "One login server is coming, id: %d", s->get_fd());
	AutoLock lock(_server_lock);
	LoginSocket* org = _servers[s->get_fd()];
	if (org && org->is_connected())
	{
		LogError("LoginServer", "Two conflict server are registed, id : %d", s->get_fd());
		org->close();
		// TODO:
	}
	_servers[s->get_fd()] = s;
}
void LoginMgr::remove_realm_server(LoginSocket* s)
{
	LogDebug("LoginServer", "One login server is lost");
	AutoLock lock(_server_lock);
	s->close();
	_servers.erase(s->get_fd());
	_servers_info.erase(s);
}

void LoginMgr::update_realm_server(LoginSocket* s, RealmSrvInfo& info)
{
	AutoLock lock(_server_lock);
	_servers_info[s] = info;
}

void LoginMgr::update_realm_server_name(LoginSocket* s, std::string name)
{
	LogDebug("LoginServer", "Login server (id:%d) name changed to : %s", s->get_fd(), name.c_str());
	AutoLock lock(_server_lock);
	_servers_info[s].name = name;
}

void LoginMgr::update_realm_server_info(LoginSocket* s, std::string info)
{
	AutoLock lock(_server_lock);
	_servers_info[s].info = info;
}

void LoginMgr::update_realm_server_status(LoginSocket* s, RealmSrvStatus status)
{
	AutoLock lock(_server_lock);
	_servers_info[s].status = status;
}

void LoginMgr::add_client(LoginSocket* s)
{
	AutoLock lock(_client_lock);
	LoginSocket* org = _clients[s->get_fd()];
	if (org && org->is_connected())
	{
		LogError("LoginServer", "Two conflict clients are registed, id : %d", s->get_fd());
		org->close();
		// TODO:
	}
	_clients[s->get_fd()] = s;
}
void LoginMgr::remove_client(LoginSocket* s)
{
	AutoLock lock(_client_lock);
	s->close();
	_clients.erase(s->get_fd());
}
