#include "loginserver.h"
#include "configfile.h"
#include "log.h"
#include "network.h"
#include "threadpool.h"
#include <iostream>
#include <vector>
#include "typedefs.h"
#include "loginmgr.h"

using namespace srdgame;

LoginServer::LoginServer(const std::string& conf_fn) : _conf_fn(conf_fn), _config(NULL), _socket(NULL), _inter_socket(NULL)
{
//	LogDebug("LoginServer", "Create the server with connfigure file:%s", conf_fn.c_str());
}

LoginServer::~LoginServer()
{
	if (_config)
	{
		delete _config;
	}
}

void LoginServer::run()
{
	if (!this->load_conf())
	{
		LogError("LoginServer", "Could not load configure file: %s", _conf_fn.c_str());
		return;
	}
	LogDebug("LoginServer", "Load configuration file completed");
	if (!this->init_env())
	{
		LogError("LoginServer", "QUIT@: Env is not ready");
	}
	if (!this->start_listen())
	{
		LogError("LoginServer", "Could not start to listen");
		return;
	}
	if (!this->start_inter_listen())
	{
		LogError("LoginServer", "Could not start the register listen");
	}
	while(!wait_command())
	{
		LogNotice(NULL, "-----------------------");
	}
}
bool LoginServer::init_env()
{
	ThreadPool::get_singleton().init(10);
	SocketMgr::get_singleton().start_worker();
	return true;
}

bool LoginServer::load_conf()
{
	if (_conf_fn.empty())
	{
		LogDebug("LoginServer", "Empty configure file name");
		return false;
	}
	LogDebug("LoginServer", "Try to load configure file: %s", _conf_fn.c_str());
	_config = new ConfigFile(_conf_fn);
	return _config->is_loaded();
}

bool LoginServer::start_listen()
{
	int port = _config->get_value<int>("PORT");
	if (port == 0)
	{
		LogWarning("LoginServer", "Invalid listen port found, change it to: %d", 7001);
		port = 7001;
	}
	std::string addr = _config->get_value<std::string>("ADDRESS");
	if (addr.empty())
	{
		addr = "127.0.0.1";
		LogWarning("LoginServer", "Invalid listen address has found");
	}

	LogDebug("LoginServer", "Try to listen on: %s %d", addr.c_str(), port);
	if (_socket)
		delete _socket;
	_socket = new TcpListenSocket<LoginSocket>(addr.c_str(), port);
	if (_socket->is_open())
	{
		LogSuccess("LoginServer", "Listening on port: %d", port);
		return true;
	}
	return false;
}
bool LoginServer::start_inter_listen()
{
	int port = _config->get_value<int>("INTER_PORT");
	if (port == 0)
	{
		port = 7101;
		LogWarning("LoginServer", "Inavlid inter port has found, change it to: %d", port);
	}
	std::string addr = _config->get_value<std::string>("INTER_ADDRESS");
	if (addr.empty())
	{
		addr = "127.0.0.1";
		LogWarning("LoginServer", "Invalid inter address has found");
	}
	LogDebug("LoginServer", "Try to inter listen on: %s %d", addr.c_str(), port);
	if (_inter_socket)
		delete _inter_socket;
	_inter_socket = new TcpListenSocket<LoginInterSocketW>(addr.c_str(), port);
	if (_inter_socket->is_open())
	{
		LogSuccess("LoginServer", "Register listening on port: %d", port);
		return true;
	}
	return false;
}

bool LoginServer::stop_listen()
{
	_socket->close();
	_inter_socket->close();
}

bool LoginServer::wait_command()
{
	string str;
	std::cin >> str;
	if (str == "quit")
	{
		return true;
	}
	else if (str == "list")
	{
		std::vector<LoginSrvInfo> info;
		LoginMgr::get_singleton().enum_world_servers(info);
		size_t i = 0;
		if (0 == info.size())
		{
			LogSuccess("LoginServer", "No world server avaliable yet!");
		}
		for (; i < info.size(); ++i)
		{
			LogSuccess("LoginServer", "World Server : %d", i);
			LogSuccess("LoginServer", "Name: %s\t Info: %s", info[i].name.c_str(), info[i].info.c_str());
			switch (info[i].status)
			{
				case WS_OFFLINE:
					LogSuccess("LoginServer", "Status: Offline");
					break;
				case WS_READY:
					LogSuccess("LoginServer", "Status: Ready");
					break;
				case WS_FULL:
					LogSuccess("LoginServer", "Status: Closing");
					break;
				case WS_TESTING:
					LogSuccess("LoginServer", "Status: Testing");
					break;
				default:
					LogSuccess("LoginServer", "Status: Unknown");
					break;
			}
		}
	}
	else
	{
		LogWarning("LoginServer", "Unknow command : %s", str.c_str());
	}
	return false;
}

