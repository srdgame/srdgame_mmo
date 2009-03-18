#include "realmsocketl.h"
#include "log.h"
#include "packetparser.h"
#include "realmworker.h"
#include "typedefs.h"
#include "realmmgr.h"
#include "realmserver.h"

using namespace srdgame;

RealmInterSocketL::RealmInterSocketL(RealmServer* server)
	: RealmSocketBase()
	  , _server(server)
{
}

RealmInterSocketL::~RealmInterSocketL()
{
	LogDebug("RealmServer", "Destructor of RealmInterSocketL (login connection)");
}

void RealmInterSocketL::on_send()
{
}

void RealmInterSocketL::on_connect()
{
}

void RealmInterSocketL::on_close()
{
	LogDebug("RealmServer", "Connection with login server has been dropdown");
	_server->lost_login();
}

void RealmInterSocketL::on_handle(Packet* packet)
{
	switch (packet->op)
	{
		case I_PING:
			// Just ping. Both side command.  Normally server will ping client per 5 seconds?
			packet->param.Int++;
			this->send_packet(packet);
			break;
		case I_OFFLINE:
			// Say goodbye, both side command.
			// TODO: What should we do when login is going to offline.
			break;
		case I_NOTIFY: // Notify others we are going online. Both side action and its param is the client type: Realm = 1, World = 2, Realm = 0,
			// TODO: What should we do when login is going to be online?
			break;
		case IS_GET_NAME: // Ask for name of client.
			{
				string name = RealmMgr::get_singleton().get_name();
				Packet p;
				p.op = IC_NAME;
				p.len = sizeof(Packet) + name.size();
				p.param.Data = name.c_str();
				send_packet(&p);
			}
			break;
		case IC_NAME:// send back the name in one string
			break;
		case IS_GET_STATUS:
			{
				Packet p;
				p.op = IC_STATUS;
				p.len = sizeof(Packet);
				p.param.Long = LS_READY;
				send_packet(&p);
			}
			break;
		case IC_STATUS: // 
			break;
		case IC_POST_STATUS:
			break;// Post the status to server

		case IS_GET_INFO:
			// ask for more detail info, the reply structure is to be defined.
			{
				/*string name = RealmMgr::get_singleton().get_info();
				Packet p;
				p.op = IC_INFO;
				p.len = sizeof(Packet) + name.size();
				p.param.Data = name.c_str();*/
				Packet p;
				p.op = IC_INFO;
				p.len = sizeof(Packet) + sizeof(RealmSrvInfo);
				RealmSrvInfo info = RealmMgr::get_singleton().get_info();
				p.param.Data = (char*)&info;
				send_packet(&p);
			}
			break;
		case IC_INFO:
			break;
		default:
			LogWarning("RealmServer", "Unknow packet is received from Login Server, the opcode is : %d", packet->op);
			break;
	}
}
