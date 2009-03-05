#include "realmserver.h"
#include "configfile.h"
#include "log.h"
#include "network.h"
#include <iostream>

using namespace srdgame;

RealmServer::RealmServer(const std::string& conf_fn) : _conf_fn(conf_fn), _config(NULL)
{
	LogDebug("RealmServer", "Create the server with connfigure file:%s\n", conf_fn.c_str());
	//printf("%s\n", conf_fn.c_str());
}

RealmServer::~RealmServer()
{
	if (_config)
	{
		delete _config;
	}
}

void RealmServer::run()
{
	if (!this->load_conf())
	{
		LogError("RealmServer", "Could not load configure file: %s", _conf_fn.c_str());
		return;
	}
	if (!this->start_listen())
	{
		LogError("RealmServer", "Could not start to listen");
	}
	while(!wait_command())
	{
		LogNotice(NULL, "-------------------------------------------------------------------------");
	}
}

bool RealmServer::load_conf()
{
	if (_conf_fn.empty())
	{
		LogDebug("RealmServer", "Empty configure file name");
		return false;
	}
	LogDebug("RealmServer", "Try to load configure file: %s", _conf_fn.c_str());
	_config = new ConfigFile(_conf_fn);
	return _config->is_loaded();
}

bool RealmServer::start_listen()
{
	int port = _config->get_value<int>("PORT");
	if (port == 0)
	{
		LogNotice("RealmServer", "Invalid port found, change it to: %d", 6001);
		port = 6001;
	}
	if (_socket)
		delete _socket;
	_socket = new TcpListenSocket<RealmSocket>("127.0.0.1", port);
	if (_socket->is_open())
	{
		LogSuccess("RealmServer", "Listening on port: %d", port);
	}
}

bool RealmServer::stop_listen()
{
	_socket->close();
}

bool RealmServer::wait_command()
{
	while (true)
	{
		string str;
		std::cin >> str;
		if (str == "quit")
		{
			break;
		}
	}
}

