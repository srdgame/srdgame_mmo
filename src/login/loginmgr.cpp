

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
void LoginMgr::enum_login_servers(std::vector<LoginSrvInfo>& info)
{
	AutoLock lock(_server_lock);
	std::map<LoginSocket*, LoginSrvInfo>::iterator ptr = _servers_info.begin();
	for (; ptr != _servers_info.end(); ++ptr)
	{
		info.push_back(ptr->second);
	}
}
void LoginMgr::add_login_server(LoginSocket* s)
{
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
void LoginMgr::remove_login_server(LoginSocket* s)
{
	AutoLock lock(_server_lock);
	s->close();
	_servers.erase(s->get_fd());
	_servers_info.erase(s);
}

void LoginMgr::update_login_server(LoginSocket* s, LoginSrvInfo& info)
{
	AutoLock lock(_server_lock);
	_servers_info[s] = info;
}

void LoginMgr::update_login_server_name(LoginSocket* s, std::string name)
{
	AutoLock lock(_server_lock);
	_servers_info[s].name = name;
}

void LoginMgr::update_login_server_info(LoginSocket* s, std::string info)
{
	AutoLock lock(_server_lock);
	_servers_info[s].info = info;
}

void LoginMgr::update_login_server_status(LoginSocket* s, LoginSrvStatus status)
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
