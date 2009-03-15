#include "loginauth.h"
#include "loginsocket.h"
#include "packetdefs.h"
#include "opcode.h"

#include "ro_defs.h"
#include "strlib.h"
#include <cstdlib>

using namespace srdgame;
using namespace srdgame::opcode;

LoginAuth::LoginAuth()
{
}

LoginAuth::~LoginAuth()
{
}

void LoginAuth::handle_login(LoginSocket* socket, const Packet* packet)
{
	LoginInfo* login_info = (LoginInfo*)packet->param.Data;
	long login_ok = 0;
	if (strcmp(login_info->name, "aaaaa.ss") || strcmp(login_info->pass, "aaaaa.f"))
	{
		login_ok = 1;
	}
	
	// Fake for now. TODO:
	Packet p;
	p.op = ES_LOGIN;
	p.len = sizeof(Packet);
	p.param.Long = login_ok;
	socket->send_packet(&p);

	if (!login_ok)
		return;

	// Send server list.
	ServerListHeader header;
	header.ip = str2ip(socket->get_remote_ip().c_str());
	header.account = 1000;
	header.id1 = rand(); // Not used I guess.
	header.id2 = rand(); // Not used I guess.
	header.sex = 'F';
	header.server_count = 1; // Only one now, it is fake.

	ServerInfo info; // fake one TODO:
	info.ip = str2ip("192.168.0.100");
	info.port = 8002;
	char  name[20] = "TEST";
	memset(info.name, 0, 20);
	memcpy(info.name, name, strlen(name));
	info.users = 1;

	char* buf = new char[sizeof(header) + sizeof(info)];
	memcpy(buf, (char*)&header, sizeof(header));
	memcpy(buf + sizeof(header), (char*)&info, sizeof(info));
	Packet listp;
	listp.op = ES_SERVER_LIST;
	listp.len = sizeof(Packet) + sizeof(header) + sizeof(info);
	listp.param.Data = buf;

	socket->send_packet(&listp);
}

void LoginAuth::handle_admin_login(LoginSocket* socket, const Packet* packet)
{
}

void LoginAuth::get_crypto_key(LoginSocket* socket, const Packet* packet)
{
}


