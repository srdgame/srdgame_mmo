#include "loginauth.h"
#include "loginsocket.h"
#include "packetdefs.h"
#include "typedefs.h"
#include "opcode.h"
#include "userinfo.h"
#include "accountmgr.h"
#include "loginmgr.h"

#include "ro_defs.h"
#include "strlib.h"
#include "rouser.h"
#include <cstdlib>
#include <vector>

using namespace srdgame;
using namespace srdgame::opcode;
using namespace ro;

#define _LOGIN_AUTH_DEBUG_
#undef _LogDebug_

#ifdef _LOGIN_AUTH_DEBUG_
#define _LogDebug_ LogDebug
#else
#define _LogDebug_ //
#endif

#undef LN
#define LN "LoginServer"

LoginAuth::LoginAuth()
{
}

LoginAuth::~LoginAuth()
{
}

void LoginAuth::handle_login(LoginSocket* socket, const Packet* packet)
{
	LoginInfo* login_info = (LoginInfo*)packet->param.Data;
	if (!login_info)
	{
		LogError(LN, "Invalid login packet are found!!!");
		return;
	}

	_LogDebug_(LN, "User login with id: %s   \t password: %s", login_info->name, login_info->pass);

	if (strlen(login_info->name) <= 2)
	{
		LogError(LN, "Can not use shorter than 2 characters for user name");
	}

	size_t index = strlen(login_info->name);
	//_LogDebug_(LN, "Strlen of Login_info->name is: %d", index);

	bool create = false;
	bool female = false;
	index--;
	if (login_info->name[index] == 'M' && login_info->name[--index] == '_')
	{
		create = true;
	}
	if (login_info->name[++index] == 'F' && login_info->name[index] == '_')
	{
		female = true;
		create = true;
	}

	if (create)
	{
		login_info->name[index--] = '\0';
		login_info->name[index] = '\0';
		LogNotice(LN, "New account are creating, name: %s  \t pass: %s", login_info->name, login_info->pass);
		create = this->create_new_account(login_info, female);
	}

	_LogDebug_(LN, "Starting to poll information from DB");

	long login_failed = 3;
	UserInfo* user_info = AccountMgr::get_singleton().load_user(login_info->name);

	_LogDebug_(LN, "Load User successfully");

	if (user_info)
	{
		_LogDebug_(LN, "Username got from db is: %s  \t password is : %s", user_info->_name.c_str(), user_info->_pass.c_str());
		if (user_info->_name == login_info->name)
		{
			if (user_info->_pass == login_info->pass)
			{
				login_failed = 0;
			}
		}
	}
	else
	{
		_LogDebug_(LN, "Failed to load user with name : %s", login_info->name);
	}


	// Fake for now. TODO:
	Packet p;
	p.op = ES_LOGIN;
	p.len = sizeof(Packet);
	p.param.Long = login_failed;
	socket->send_packet(&p);

	if (!login_failed)
	{
		lead_to_realm(socket);
	}
}
void LoginAuth::handle_admin_login(LoginSocket* socket, const Packet* packet)
{
}

void LoginAuth::get_crypto_key(LoginSocket* socket, const Packet* packet)
{
}

bool LoginAuth::create_new_account(LoginInfo* info, bool female)
{
	UserInfoEx< RoUser > user_info;
	user_info._name = info->name;
	user_info._pass = info->pass;
	user_info._id = 0;
	RoUser* user = user_info.get_ex_data();
	memset((char*)user, 0, sizeof(RoUser));
	user_info.get_ex_data()->_sex = female ? 'F' : 'M';
	AccountMgr::get_instance()->create_new(&user_info);
	return false;
}

void LoginAuth::lead_to_realm(LoginSocket* socket)
{
	std::vector<RealmSrvInfo> servers;
	LoginMgr::get_singleton().enum_realm_servers(servers);

	size_t size = servers.size();

	// Send server list.
	ServerListHeader header;
	header.ip = str2ip(socket->get_remote_ip().c_str());
	header.account = 1000;
	header.id1 = rand(); // Not used I guess.
	header.id2 = rand(); // Not used I guess.
	header.sex = 'F';
	header.server_count = (uint32)size; // Only one now, it is fake.


	char* buf = new char[sizeof(header) + sizeof(ServerInfo) * size];
	memcpy(buf, (char*)&header, sizeof(header));

	for( size_t i = 0; i < size; ++i)
	{
		ServerInfo info; // fake one TODO:
		info.ip = str2ip(servers[i].ip);
		info.port = (uint16)servers[i].port;
		memset(info.name, 0, 20);
		memcpy(info.name, servers[i].name, 20);
		info.users = 1;
	
		// Copy to buffer.
		memcpy(buf + sizeof(header) + sizeof(info) * i, (char*)&info, sizeof(info));
	}

	// Send packetage.
	Packet listp;
	listp.op = ES_SERVER_LIST;
	listp.len = sizeof(Packet) + sizeof(header) + sizeof(ServerInfo);
	listp.param.Data = buf;

	socket->send_packet(&listp);
}

