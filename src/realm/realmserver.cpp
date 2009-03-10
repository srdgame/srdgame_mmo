#include "realmserver.h"
#include "configfile.h"
#include "log.h"
#include "network.h"
#include "threadpool.h"
#include <iostream>
#include <vector>
#include "typedefs.h"
#include "realmmgr.h"

using namespace srdgame;

RealmServer::RealmServer(const std::string& conf_fn) : _conf_fn(conf_fn), _config(NULL), _socket(NULL), _inter_socket(NULL)
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
	if (!this->start_inter_listen())
	{
		LogError("RealmServer", "Could not start the register listen");
	}
	while(!wait_command())
	{
		LogNotice(NULL, "-----------------------");
	}
}
bool RealmServer::init_env()
{
	ThreadPool::get_singleton().init(10);
	SocketMgr::get_singleton().start_worker();
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
		LogWarning("RealmServer", "Invalid port found, change it to: %d", 6001);
		port = 6001;
	}
	std::string addr = _config->get_value<std::string>("ADDRESS");
	if (addr.empty())
	{
		addr = "127.0.0.1";
		LogWarning("RealmServer", "Invalid inter address has found");
	}

	LogDebug("RealmServer", "Try to listen on: %s %d", addr.c_str(), port);
	if (_socket)
		delete _socket;
	_socket = new TcpListenSocket<RealmSocket>(addr.c_str(), port);
	if (_socket->is_open())
	{
		LogSuccess("RealmServer", "Listening on port: %d", port);
		return true;
	}
	return false;
}
bool RealmServer::start_inter_listen()
{
	int port = _config->get_value<int>("INTER_PORT");
	if (port == 0)
	{
		port = 6101;
		LogWarning("RealmServer", "Inavlid inter port has found, change it to: %d", port);
	}
	std::string addr = _config->get_value<std::string>("INTER_ADDRESS");
	if (addr.empty())
	{
		addr = "127.0.0.1";
		LogWarning("RealmServer", "Invalid inter address has found");
	}
	LogDebug("RealmServer", "Try to inter listen on: %s %d", addr.c_str(), port);
	if (_inter_socket)
		delete _inter_socket;
	_inter_socket = new TcpListenSocket<RealmSocketInter>(addr.c_str(), port);
	if (_inter_socket->is_open())
	{
		LogSuccess("RealmServer", "Register listening on port: %d", port);
		return true;
	}
	return false;
}

bool RealmServer::stop_listen()
{
	if (_socket)
		_socket->close();
	if (_inter_socket)
		_inter_socket->close();
}

bool RealmServer::wait_command()
{
	string str;
	std::cin >> str;
	if (str == "quit")
	{
		return true;
	}
	else if (str == "help")
	{
		LogNotice("RealmServer", "Avaliable commands:");
		LogSuccess("RealmServer", "quit --- stop and quit this server");
		LogSuccess("RealmServer", "list --- list all the login servers");
		LogSuccess("RealmServer", "help --- list all the avaliable commands");
	}
	else if (str == "list")
	{
		std::vector<LoginSrvInfo> info;
		RealmMgr::get_singleton().enum_login_servers(info);
		size_t i = 0;
		if (0 == info.size())
		{
			LogSuccess("RealmServer", "No login server avaliable yet!");
		}
		for (; i < info.size(); ++i)
		{
			LogSuccess("RealmServer", "Login Server : %d", i);
			LogSuccess("RealmServer", "Name: %s\t Info: %s", info[i].name.c_str(), info[i].info.c_str());
			switch (info[i].status)
			{
				case LS_OFFLINE:
					LogSuccess("RealmServer", "Status: Offline");
					break;
				case LS_READY:
					LogSuccess("RealmServer", "Status: Ready");
					break;
				case LS_FULL:
					LogSuccess("RealmServer", "Status: Closing");
					break;
				case LS_TESTING:
					LogSuccess("RealmServer", "Status: Testing");
					break;
				default:
					LogSuccess("RealmServer", "Status: Unknown");
					break;
			}
		}
	}
	else
	{
		LogWarning("RealmServer", "Unknow command : %s", str.c_str());
	}
	return false;
}

