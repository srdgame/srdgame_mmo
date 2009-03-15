#ifndef REALM_SERVER_H_
#define REALM_SERVER_H_

#include <string>
//#include "configfile.h"
#include "realmsocket.h"
#include "realmsocketw.h"
#include "network.h"

namespace srdgame
{
class RealmInterSocketL;
class ConfigFile;
class RealmServer
{
public:
	RealmServer(const std::string& conf_fn);
	virtual ~RealmServer();

	// methods
	void run();

	void lost_realm();

protected:
	bool load_conf();
	bool init_packet_parser();
	bool init_env();
	bool start_listen();
	bool start_inter_listen();
	bool connect_login();

	// return true to quit.
	bool wait_command();

protected:
	std::string _conf_fn;
	ConfigFile* _config;
	TcpListenSocket< RealmSocket > * _socket;
	TcpListenSocket< RealmInterSocketW > * _inter_socket;
	RealmInterSocketL* _login_socket;

	//SmartPtr< TcpListenSocket< RealmSocket > > _socket;
	//SmartPtr< TcpListenSocket< RealmInterSocketW > > _inter_socket;
//	SmartPtr< RealmInterSocketL > _login_socket;
};
}

#endif
