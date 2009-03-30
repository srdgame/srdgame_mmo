#include "worldauth.h"
#include "worldsocket.h"
#include "packetdefs.h"
#include "typedefs.h"
#include "opcode.h"

#include "ro_defs.h"
#include "strlib.h"
#include "rouser.h"
#include "rocharinfo.h"
#include <cstdlib>
#include <vector>

#include "timedefs.h"
#include "log.h"
#include "mapmgr.h"
#include "map.h"
#include "player.h"

using namespace srdgame;
using namespace srdgame::opcode;
using namespace ro;

#define _WORLD_AUTH_DEBUG_
#undef _LogDebug_

#ifdef _WORLD_AUTH_DEBUG_
#define _LogDebug_ LogDebug
#else
#define _LogDebug_ //
#endif

#undef LN
#define LN "WorldServer"

WorldAuth::WorldAuth()
{
}

WorldAuth::~WorldAuth()
{
}

void WorldAuth::handle_login(WorldSocket* socket, const Packet* packet)
{
	_LogDebug_(LN, "Handle login !!!!!!!!!!!!!!!!!");
	if (packet->len <= sizeof(Packet))
		return;
	ConnectToMap* c = (ConnectToMap*)packet->param.Data;

	Packet p;
	p.op = (ES_CONNECT_TO_MAP);
	p.len = sizeof(Packet);
	_LogDebug_(LN, "Sending back the account : %d", c->_account_id);
	p.param.Int = c->_account_id;
	//socket->send_packet(&p);

	if(this->auth_char(socket))
	{
		Player * p = new Player(socket, c->_account_id, c->_char_id);
		reg_to_map(p);
	}
	else
	{
		// TODO: reject connection
	}
}
void WorldAuth::handle_admin_login(WorldSocket* socket, const Packet* packet)
{
}
bool WorldAuth::auth_char(WorldSocket* socket)
{
	_LogDebug_(LN, "Handle auth_char");
	Packet p;
	p.op = ES_MAP_AUTH;
	p.len = sizeof(Packet) + sizeof(MapAuthOK);
	MapAuthOK* ok = new MapAuthOK();
	ok->_tick_count = gettick();
	ok->_x = 53;
	ok->_y = 111;
	ok->_dir = 6;
	p.param.Data = (char*)ok;
	socket->send_packet(&p);
	return true;
}
void WorldAuth::reg_to_map(Player* p)
{
	std::string map_name = p->get_last_map();
	Map* map = MapMgr::get_singleton().get_map(map_name);
	if (map->add_player(p))
	{
		LogSuccess(LN, "One player has appear to map : %s", p->get_last_map().c_str());
		p->set_map(map);
	}
	else
	{
		// TODO:
	}
}