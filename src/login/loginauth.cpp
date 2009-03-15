#include "loginauth.h"
#include "loginsocket.h"
#include "packetdefs.h"
#include "opcode.h"

#include "ro_defs.h"
#include "strlib.h"

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
	// Fake for now. TODO:
	Packet p;
	p.op = ES_LOGIN;
	p.len = sizeof(Packet);
	p.param.Long = 0;
	socket->send_packet(&p);

	// Send server list.
	ServerListHeader header;
	header.ip = str2ip(socket->get_remote_ip().c_str());
	header.account = 1000;
	header.id1 = 0;
	header.id2 = 1;
	header.sex = 'F';
	header.server_count = 1;

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

// Resolves hostname into a numeric ip.
uint32 host2ip(const char* hostname)
{
	struct hostent* h = gethostbyname(hostname);
	return (h != NULL) ? ntohl(*(uint32*)h->h_addr) : 0;
}

// Converts a numeric ip into a dot-formatted string.
// Result is placed either into a user-provided buffer or a static system buffer.
const char* ip2str(uint32 ip, char ip_str[16])
{
	struct in_addr addr;
	addr.s_addr = htonl(ip);
	return (ip_str == NULL) ? inet_ntoa(addr) : strncpy(ip_str, inet_ntoa(addr), 16);
}

// Converts a dot-formatted ip string into a numeric ip.
uint32 str2ip(const char* ip_str)
{
	return ntohl(inet_addr(ip_str));
}

