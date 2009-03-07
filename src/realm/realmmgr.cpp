

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

void RealmMgr::add_login_server(RealmSocket* s)
{
	AutoLock lock(_server_lock);
	RealmSocket* org = _servers[s->get_fd()];
	if (org && org->is_connected())
	{
		LogError("RealmServer", "Two conflict server are registed, id : %d", s->get_fd());
		org->close();
		// TODO:
	}
	_servers[s->get_fd()] = s;
}
void RealmMgr::remove_login_server(RealmSocket* s)
{
	AutoLock lock(_server_lock);
	s->close();
	_servers.erase(s->get_fd());
	_servers_info.erase(s);
}

void RealmMgr::update_login_server(RealmSocket* s, LoginSrvInfo& info)
{
	AutoLock lock(_server_lock);
	_servers_info[s] = info;
}

void RealmMgr::add_client(RealmSocket* s)
{
	AutoLock lock(_client_lock);
	RealmSocket* org = _clients[s->get_fd()];
	if (org && org->is_connected())
	{
		LogError("RealmServer", "Two conflict clients are registed, id : %d", s->get_fd());
		org->close();
		// TODO:
	}
	_clients[s->get_fd()] = s;
}
void RealmMgr::remove_client(RealmSocket* s)
{
	AutoLock lock(_client_lock);
	s->close();
	_clients.erase(s->get_fd());
}
