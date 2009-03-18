

#include "worldmgr.h"
#include "worldsocket.h"

#include "log.h"
#include "autolock.h"

using namespace srdgame;

WorldMgr::WorldMgr()
{
}
WorldMgr::~WorldMgr()
{
}

void WorldMgr::add_client(WorldSocketBase* s)
{
	AutoLock lock(_client_lock);
	WorldSocketBase* org = _clients[s->get_fd()];
	if (org && org->is_connected())
	{
		LogError("WorldServer", "Two conflict clients are registed, id : %d", s->get_fd());
		org->close();
		// TODO:
	}
	_clients[s->get_fd()] = s;
}
void WorldMgr::remove_client(WorldSocketBase* s)
{
	AutoLock lock(_client_lock);
	s->close();
	_clients.erase(s->get_fd());
}

void WorldMgr::set_name(std::string name)
{
	AutoLock lock(_lock);
	::memcpy(_info.name, name.c_str(), name.size());;
}
std::string WorldMgr::get_name()
{
	AutoLock lock(_lock);
	return string(this->_info.name);
}

void WorldMgr::set_info(WorldSrvInfo& info)
{
	AutoLock lock(_lock);
	this->_info = info;
}
WorldSrvInfo& WorldMgr::get_info()
{	
	AutoLock lock(_lock);
	return this->_info;
}
