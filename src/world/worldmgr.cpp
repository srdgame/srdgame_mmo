
#include "worldmgr.h"
#include "worldsocket.h"
#include "mapmgr.h"
#include "map.h"

#include "log.h"
#include "autolock.h"
#include "player.h"
#include "opcode.h"

#include "ro_defs.h"

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

WorldMgr::WorldMgr() : _sql(NULL)
{
}
WorldMgr::~WorldMgr()
{
}

void WorldMgr::init()
{
	_sql = new RoSql();
}

void WorldMgr::add_client(Player* s)
{
	AutoLock lock(_client_lock);
	Player* org = _clients[s->get_id()];
	if (org)
	{
		LogError("WorldServer", "Two conflict players are registed, id : %d", s->get_id());
		delete org;
		// TODO:
	}

	if (!setup_player(s))
	{
		delete s;
		return;
	}
	if (!reg_to_map(s))
	{
		delete s;
		return;
	}

	_clients[s->get_id()] = s;

	game_tips(s);
}
void WorldMgr::remove_client(Player* s)
{
	AutoLock lock(_client_lock);
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
		// TODO:
		return false;
	}
}
void WorldMgr::game_tips(Player* s)
{
	// Server version message.
	char buf[256];
	sprintf(buf, "SRD Game version : 0.0.1");
	Packet p(ES_MESSAGE);
	p.len = sizeof(Packet) + strlen(buf);
	p.param.Data = buf;
	s->send_packet(&p);

	// Message of the Day?
	sprintf(buf, "Private message test!!!");
	p.op = ES_PRIVATE_MSG;
	p.len = sizeof(Packet) + strlen(buf);
	s->send_packet(&p);

	// Time limited.
	WisMessage msg;
	sprintf(msg._name, "Server");
	sprintf(msg._msg, "No time limitation performed on srdgame server");
	p.op = ES_WIS_MESSAGE;
	p.len = sizeof(Packet) + sizeof(WisMessage);
	p.param.Data = (char*)&msg;
	s->send_packet(&p);

	// Night message
	sprintf(msg._msg, "NIGHTLY Message");
	s->send_packet(&p);

	// TODO: MOre.
}
