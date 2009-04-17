
#include "worldmgr.h"
#include "worldsocket.h"
#include "mapmgr.h"
#include "map.h"

#include "log.h"
#include "autolock.h"
#include "player.h"
#include "opcode.h"
#include "worldupdater.h"

#include "ro_defs.h"
#include "roitemdb.h"

using namespace ro;
using namespace srdgame;
using namespace srdgame::opcode;

#define _WORLD_MGR_DEBUG_
#undef _LogDebug_

#ifdef _WORLD_MGR_DEBUG_
#define _LogDebug_ LogDebug
#else
#define _LogDebug_ //
#endif

#undef LN
#define LN "WorldServer"

WorldMgr::WorldMgr() : _inited(false), _updater(NULL), _sql(NULL)
{
}
WorldMgr::~WorldMgr()
{
	if(_inited)
	{
		delete _updater;
		delete _sql;
	}
}

void WorldMgr::init()
{
	AutoLock lock(_lock);
	if (_inited)
		return;
	_inited = true;
	_updater = new WorldUpdater();
	_sql = new RoSql();

	RoItemDB::get_singleton().init(); // init the item db // need a lot time?
}

void WorldMgr::add_client(Player* s)
{
	{
		// Lock the client only we are accessing the _clients, to make sure, we will not block other players' connection
		AutoLock lock(_client_lock);
		Player* org = _clients[s->get_id()];
		if (org)
		{
			LogError("WorldServer", "Two conflict players are registed, id : %d", s->get_id());
			delete org;
			// TODO:
		}
	}

	// Setup means load all player's information from DB.
	if (!setup_player(s))
	{
		LogError("WorlsServer", "Failed to set up players, mostly it is error when query DB");
		delete s;
		return;
	}
	// Try to register the player to the map.
	if (!reg_to_map(s))
	{
		LogError("WorldServer", "Could not register player to server");
		delete s;
		return;
	}

	{
		// Same as above.
		AutoLock lock(_client_lock);
		_clients[s->get_id()] = s;
	}

	game_tips(s);
}
void WorldMgr::remove_client(Player* s)
{
	if (!s)
		return;
	AutoLock lock(_client_lock);
	save_player(s);
	remove_from_map(s);
	delete s;
	_clients.erase(s->get_id());
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
Updater* WorldMgr::get_updater()
{
	return _updater;
}
bool WorldMgr::setup_player(Player* p)
{
	if (!_sql)
		return false;
	RoCharInfo* info = new RoCharInfo();
	bool res = _sql->load_char(p->get_id(), *info, true);
	if (res)
	{
		//_LogDebug_(LN, "Now set_info to player, &info %d", info);
		p->set_info(info);
	}
	return res;
}
void WorldMgr::save_player(Player* p)
{
	if (!_sql)
		return;
	_sql->save_char(p->get_id(), *((RoCharInfo*)p->get_info()));

}
bool WorldMgr::reg_to_map(Player* p)
{
	std::string map_name = p->get_last_map();
	Map* map = MapMgr::get_singleton().get_map(map_name);
	if (map->add_player(p))
	{
		LogSuccess(LN, "One player has appear to map : %s", p->get_last_map().c_str());
		p->set_map(map);
		return true;
	}
	else
	{
		// TODO:  Now return false, means focus close the connection to reject the connecting.
		return false;
	}
}
void WorldMgr::remove_from_map(Player* p)
{
	std::string map_name = p->get_last_map();
	Map* map = MapMgr::get_singleton().get_map(map_name);
	if (map)
	{
		map->remove_player(p);
	}
}
void WorldMgr::game_tips(Player* s)
{
	// Server version message.
	char buf[256];
	sprintf(buf, "SRD Game version : 0.0.1");
	Packet p(ES_MESSAGE);
	RoMessage m;
	m._len = strlen(buf);
	m._msg = buf;
	m._id = 0;
	p.len = sizeof(Packet) + sizeof(RoMessage);
	p.param.Data = (char*)&m;
	s->send_packet(&p);

	// Message of the Day?
	sprintf(buf, "Private message test!!!");
	p.op = ES_PRIVATE_MSG;
	p.len = sizeof(Packet) + strlen(buf);
	s->send_packet(&p);

	// Time limited.
	WisMessage msg;
	sprintf(msg._name, "Server");
	sprintf(msg._msg, "SRD Game server is free to play forever!!!");
	p.op = ES_WIS_MESSAGE;
	p.len = sizeof(Packet) + sizeof(WisMessage);
	p.param.Data = (char*)&msg;
	s->send_packet(&p);

	// Night message
	sprintf(msg._msg, "SRD Game server is free to play forever!!!");
	s->send_packet(&p);

	// TODO: MOre.
}
