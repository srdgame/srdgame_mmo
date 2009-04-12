#include "gmsg.h"
#include "player.h"
#include "mapmgr.h"
#include "command.h"
#include "log.h"

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
	send(-1, msg, 0);
}
void GMsg::send(int mid, const char* msg)
{
	send(mid, msg, 0);
}
void GMsg::send(const char* msg, const Player* player)
{
	send(-1, msg, player);
}
void GMsg::send(int mid, const char* msg, const Player* player)
{
	const char* c_msg = msg;
	while (*c_msg == ' ')
	{
		c_msg++;
	}
	LogDebug("GMessage", "Sending : %s", msg);
	if (*c_msg == '.')
	{
		Command::get_singleton().handle(player, c_msg);
	}
	else
	{
		MSG m(mid, msg, player->get_id());
		_msgs.push(m);
	}
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

