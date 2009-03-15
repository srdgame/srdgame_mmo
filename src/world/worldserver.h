#ifndef LOGIN_SERVER_H_
#define LOGIN_SERVER_H_

#include <string>
//#include "configfile.h"
#include "worldsocket.h"
#include "network.h"

namespace srdgame
{
class InterSocket;
class ConfigFile;
class WorldServer
{
public:
	WorldServer(const std::string& conf_fn);
	virtual ~WorldServer();

	// methods
	void run();

	void lost_realm();

protected:
	bool load_conf();
	bool init_packet_parser();
	bool init_env();
	bool start_listen();
	bool start_inter_listen();
	bool connect_realm();

	// return true to quit.
	bool wait_command();

protected:
	std::string _conf_fn;
	ConfigFile* _config;
	TcpListenSocket< WorldSocket > * _socket;
	InterSocket* _realm_socket;

	//SmartPtr< TcpListenSocket< WorldSocket > > _socket;
	//SmartPtr< TcpListenSocket< WorldInterSocketW > > _inter_socket;
//	SmartPtr< WorldInterSocketR > _realm_socket;
};
}

#endif
