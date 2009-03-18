#include "loginserver.h"
#include "configfile.h"
#include "log.h"
#include "network.h"
#include "threadpool.h"
#include <iostream>
#include <vector>
#include "typedefs.h"
#include "loginmgr.h"
#include "packetparser.h"
#include "databasemgr.h"

using namespace srdgame;

LoginServer::LoginServer(const std::string& conf_fn) : _conf_fn(conf_fn), _config(NULL), _socket(NULL), _inter_socket(NULL)
{
//	LogDebug("LoginServer", "Create the server with connfigure file:%s", conf_fn.c_str());
}

LoginServer::~LoginServer()
{
	LogDebug("LoginServer", "Deleting LoginServer");
	if (_config)
	{
		delete _config;
	}
	if (_socket)
	{
		delete _socket;
	}
	if (_inter_socket)
	{
		delete _inter_socket;
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
	if (!this->init_packet_parser())
	{
		LogError("WorldServer", "Could not load protocol so file");
		return;
	}

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
	DatabaseMgr::get_singleton().init(_config);
	return true;
}
bool LoginServer::init_packet_parser()
{
	return PacketParser::get_singleton().init_ex(_config->get_value<std::string>("EX_PROTOCOL_LIB").c_str());
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
		LogWarning("LoginServer", "Invalid port found, change it to: %d", 6001);
		port = 6001;
	}
	std::string addr = _config->get_value<std::string>("ADDRESS");
	if (addr.empty())
	{
		addr = "127.0.0.1";
		LogWarning("LoginServer", "Invalid inter address has found");
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
		port = 6101;
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
	_inter_socket = new TcpListenSocket<InterSocket>(addr.c_str(), port);
	if (_inter_socket->is_open())
	{
		LogSuccess("LoginServer", "Register listening on port: %d", port);
		return true;
	}
	return false;
}



bool LoginServer::wait_command()
{
	string str;
	std::cin >> str;
	if (str == "quit")
	{
		ThreadPool::get_singleton().shutdown();
		return true;
	}
	else if (str == "help")
	{
		LogNotice("LoginServer", "Avaliable commands:");
		LogSuccess("LoginServer", "quit --- stop and quit this server");
		LogSuccess("LoginServer", "list --- list all the login servers");
		LogSuccess("LoginServer", "help --- list all the avaliable commands");
	}
	else if (str == "list")
	{
		std::vector<RealmSrvInfo> info;
		LoginMgr::get_singleton().enum_realm_servers(info);
		size_t i = 0;
		if (0 == info.size())
		{
			LogSuccess("LoginServer", "No login server avaliable yet!");
		}
		for (; i < info.size(); ++i)
		{
			LogSuccess("LoginServer", "Login Server : %d", i);
			LogSuccess("LoginServer", "Name: %s\t Info: %s", info[i].name, info[i].info);
			switch (info[i].status)
			{
				case LS_OFFLINE:
					LogSuccess("LoginServer", "Status: Offline");
					break;
				case LS_READY:
					LogSuccess("LoginServer", "Status: Ready");
					break;
				case LS_FULL:
					LogSuccess("LoginServer", "Status: Closing");
					break;
				case LS_TESTING:
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

