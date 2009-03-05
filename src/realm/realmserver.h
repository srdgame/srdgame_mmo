#ifndef REALM_SERVER_H_
#define REALM_SERVER_H_

#include <string>
//#include "configfile.h"
#include "realmsocket.h"
#include "network.h"

namespace srdgame
{
class ConfigFile;
class RealmServer
{
public:
	RealmServer(const char* conf_fn);
	virtual ~RealmServer();

	// methods
	void run();

protected:
	bool load_conf();
	bool start_listen();
	bool stop_listen();

	// return true to quit.
	bool wait_command();

protected:
	std::string _conf_fn;
	ConfigFile* _config;
	TcpListenSocket<RealmSocket>* _socket;
};
}

#endif
