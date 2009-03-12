#ifndef LOGIN_SERVER_H_
#define LOGIN_SERVER_H_

#include <string>
//#include "configfile.h"
#include "loginsocket.h"
#include "loginsocketw.h"
#include "network.h"

namespace srdgame
{
class LoginInterSocketR;
class ConfigFile;
class LoginServer
{
public:
	LoginServer(const std::string& conf_fn);
	virtual ~LoginServer();

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
	TcpListenSocket< LoginSocket > * _socket;
	TcpListenSocket< LoginInterSocketW > * _inter_socket;
	LoginInterSocketR* _realm_socket;

	//SmartPtr< TcpListenSocket< LoginSocket > > _socket;
	//SmartPtr< TcpListenSocket< LoginInterSocketW > > _inter_socket;
//	SmartPtr< LoginInterSocketR > _realm_socket;
};
}

#endif
