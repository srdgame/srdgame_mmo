#include "realmserver.h"
#include "configfile.h"
#include "log.h"
#include "network.h"
#include "threadpool.h"
#include <iostream>

using namespace srdgame;

RealmServer::RealmServer(const std::string& conf_fn) : _conf_fn(conf_fn), _config(NULL), _socket(NULL)
{
//	LogDebug("RealmServer", "Create the server with connfigure file:%s", conf_fn.c_str());
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
	LogDebug("RealmServer", "Load configuration file completed");
	if (!this->init_env())
	{
		LogError("RealmServer", "QUIT@: Env is not ready");
	}
	if (!this->start_listen())
	{
		LogError("RealmServer", "Could not start to listen");
		return;
	}
	while(!wait_command())
	{
		LogNotice(NULL, "-----------------------");
	}
}
bool RealmServer::init_env()
{
	ThreadPool::get_singleton().init(10);
	SocketManager::get_singleton().start_worker();
	return true;
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
	LogDebug("RealmServer", "Try to listen on: %d", port);
	if (_socket)
		delete _socket;
	_socket = new TcpListenSocket<RealmSocket>("127.0.0.1", port);
	if (_socket->is_open())
	{
		LogSuccess("RealmServer", "Listening on port: %d", port);
		return true;
	}
	return false;
}

bool RealmServer::stop_listen()
{
	_socket->close();
}

bool RealmServer::wait_command()
{
	string str;
	std::cin >> str;
	if (str == "quit")
	{
		return true;
	}
	return false;
}

