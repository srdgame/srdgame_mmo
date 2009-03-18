

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
	std::map<LoginSocketBase*, RealmSrvInfo>::iterator ptr = _servers_info.begin();
	for (; ptr != _servers_info.end(); ++ptr)
	{
		info.push_back(ptr->second);
	}
}
void LoginMgr::add_realm_server(LoginSocketBase* s)
{
	LogDebug("LoginServer", "One login server is coming, id: %d", s->get_fd());
	AutoLock lock(_server_lock);
	LoginSocketBase* org = _servers[s->get_fd()];
	if (org && org->is_connected())
	{
		LogError("LoginServer", "Two conflict server are registed, id : %d", s->get_fd());
		//org->close();
		// TODO:
	}
	_servers[s->get_fd()] = s;
}
void LoginMgr::remove_realm_server(LoginSocketBase* s)
{
	LogDebug("LoginServer", "One realm server is lost");
	AutoLock lock(_server_lock);
//	s->close(); do not call this, we do not have the right to close it
	if (_servers.find(s->get_fd()) != _servers.end())
	{
		_servers.erase(s->get_fd());
	}
	if (_servers_info.find(s) != _servers_info.end())
	{
		_servers_info.erase(s);
	}
}

void LoginMgr::update_realm_server(LoginSocketBase* s, RealmSrvInfo& info)
{
	AutoLock lock(_server_lock);
	_servers_info[s] = info;
}

void LoginMgr::update_realm_server_name(LoginSocketBase* s, std::string name)
{
	LogDebug("LoginServer", "Login server (id:%d) name changed to : %s", s->get_fd(), name.c_str());
	AutoLock lock(_server_lock);
	::memcpy(_servers_info[s].name, name.c_str(), name.size());
}

void LoginMgr::update_realm_server_status(LoginSocketBase* s, RealmSrvStatus status)
{
	AutoLock lock(_server_lock);
	_servers_info[s].status = status;
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
	//s->close();
	_clients.erase(s->get_fd());
}
