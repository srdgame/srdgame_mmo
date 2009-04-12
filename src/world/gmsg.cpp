#include "gmsg.h"
#include "player.h"
#include "mapmgr.h"

using namespace srdgame;
using namespace std;

GMsg::GMsg()
{
}
GMsg::~GMsg()
{
}
void GMsg::send(const char* msg)
{
	MSG m(-1, msg, 0);
	_msgs.push(m);
}
void GMsg::send(int mid, const char* msg)
{
	MSG m(mid, msg, 0);
	_msgs.push(m);
}
void GMsg::send(const char* msg, const Player* player)
{
	MSG m(-1, msg, player->get_id());
	_msgs.push(m);
}
void GMsg::send(int mid, const char* msg, const Player* player)
{
	MSG m(mid, msg, player->get_id());
	_msgs.push(m);
}

long GMsg::notify(long time)
{
	MSG msg;
	while (_msgs.try_pop(msg))
	{
		// send message.
		MapMgr::get_singleton().send_msg(msg._mid, *msg._msg, msg._pid);
		// delete 
		delete msg._msg;
	}
}

