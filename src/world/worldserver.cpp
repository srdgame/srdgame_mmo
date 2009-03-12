#include "worldserver.h"
#include "configfile.h"
#include "log.h"
#include "network.h"
#include "threadpool.h"
#include <iostream>
#include <vector>
#include "typedefs.h"
#include "worldmgr.h"
#include "intersocket.h"
#include "packetparser.h"

using namespace srdgame;

WorldServer::WorldServer(const std::string& conf_fn) : _conf_fn(conf_fn), _config(NULL), _socket(NULL), _login_socket(NULL)
{
//	LogDebug("WorldServer", "Create the server with connfigure file:%s", conf_fn.c_str());
}

WorldServer::~WorldServer()
{
	//LogDebug("WorldServer", "Destructor of WorldServer");
	if (_config)
	{
	//	LogDebug("WorldServer", "Deleting config");
		delete _config;
		_config = NULL;
	//	LogDebug("WorldServer", "Deleted config");
	}
	//std::string input;
	//std::cin >> input;
	//LogDebug("WorldServer", "Stoping sockets");
	if (_socket)
		delete _socket;
}

void WorldServer::run()
{
	if (!this->load_conf())
	{
		LogError("WorldServer", "Could not load configure file: %s", _conf_fn.c_str());
		return;
	}
	LogDebug("WorldServer", "Load configuration file completed");
	if (!this->init_packet_parser())
	{
		LogError("WorldServer", "Could not load protocol so file");
		return;
	}
	if (!this->init_env())
	{
		LogError("WorldServer", "QUIT@: Env is not ready");
	}
	if (!this->start_listen())
	{
		LogError("WorldServer", "Could not start to listen");
		return;
	}
	if (!this->connect_login())
	{
		LogError("WorldServer", "Could not connect to login server");
	}
	LogNotice(NULL, "-------------------------------------------------");
	while(!wait_command())
	{
		LogNotice(NULL, "-----------------------");
	}
}
void WorldServer::lost_login()
{
	_login_socket = NULL;
}
bool WorldServer::init_packet_parser()
{
	return PacketParser::get_singleton().init_ex(_config->get_value<std::string>("EX_PROTOCOL_LIB").c_str());
}
bool WorldServer::init_env()
{
	ThreadPool::get_singleton().init(10);
	SocketMgr::get_singleton().start_worker();
	WorldMgr::get_singleton().set_name(_config->get_value<std::string>("NAME"));
	WorldMgr::get_singleton().set_info(_config->get_value<std::string>("INFO"));
	return true;
}

bool WorldServer::load_conf()
{
	if (_conf_fn.empty())
	{
		LogDebug("WorldServer", "Empty configure file name");
		return false;
	}
	LogDebug("WorldServer", "Try to load configure file: %s", _conf_fn.c_str());
	_config = new ConfigFile(_conf_fn);
	return _config->is_loaded();
}

bool WorldServer::start_listen()
{
	int port = _config->get_value<int>("PORT");
	if (port <= 0)
	{
		LogWarning("WorldServer", "Invalid listen port found, change it to: %d", 7001);
		port = 7001;
	}
	std::string addr = _config->get_value<std::string>("ADDRESS");
	if (addr.empty())
	{
		addr = "127.0.0.1";
		LogWarning("WorldServer", "Invalid listen address has found");
	}

	LogDebug("WorldServer", "Try to listen on: %s %d", addr.c_str(), port);
	_socket = new TcpListenSocket<WorldSocket>(addr.c_str(), port);
	if (_socket->is_open())
	{
		LogSuccess("WorldServer", "Listening on port: %d", port);
		return true;
	}
	return false;
}


bool WorldServer::connect_login()
{
	int has_login = _config->get_value<int>("ENABLE_LOGIN");
	if (has_login == 0)
	{
		// There is no login server.
		return true;
	}
	int port = _config->get_value<int>("LOGIN_PORT");
	if (0 >= port)
	{
		port = 6101;
		LogWarning("WorldServer", "Invalid login server port has found, adjust to: %d", port);
	}
	std::string addr = _config->get_value<std::string>("LOGIN_ADDRESS");
	if (addr.empty())
	{
		addr = "127.0.0.1";
		LogWarning("WorldServer", "Can not found login server address, use default: %s", addr.c_str());
	}
	LogDebug("WorldServer", "Try to connect to login server : %s %d", addr.c_str(), port);
	if (_login_socket)
	{
		_login_socket->close();
	}
	_login_socket = new InterSocket(this);
	if (!_login_socket->connect(addr, port))
	{
		LogDebug("WorldServer", "Could not connect to login server, please start login server first or correct your configuration file");
		return false;
	}
	
	Packet p;
	p.op = I_NOTIFY;
	p.len = sizeof(Packet);
	LogDebug("WorldServer", "sizeof(Packet) is %d", sizeof(Packet));
	p.param.Long = 2;
	_login_socket->send_packet(&p);
	//_login_socket->send_packet(&p);
	
	return true;
}


bool WorldServer::wait_command()
{
	string str;
	std::cin >> str;
	if (str == "quit")
	{
		Packet of;
		of.op = I_OFFLINE;
		of.len = sizeof(Packet);
		of.param.Long = 0;
		if (_login_socket && _login_socket->is_connected())
			_login_socket->send_packet(&of);
		ThreadPool::get_singleton().shutdown();
		return true;
	}
	else if (str == "list")
	{
		/*
		std::vector<UserInfo> info;
		WorldMgr::get_singleton().enum_user_servers(info);
		size_t i = 0;
		if (0 == info.size())
		{
			LogSuccess("WorldServer", "No user is playing!");
		}
		for (; i < info.size(); ++i)
		{
			LogSuccess("WorldServer", "User : %d", i);
			LogSuccess("WorldServer", "Name: %s\t Info: %s", info[i].name.c_str(), info[i].info.c_str());
			switch (info[i].status)
			{
				case WS_OFFLINE:
					LogSuccess("WorldServer", "Status: Offline");
					break;
				case WS_READY:
					LogSuccess("WorldServer", "Status: Ready");
					break;
				case WS_FULL:
					LogSuccess("WorldServer", "Status: Closing");
					break;
				case WS_TESTING:
					LogSuccess("WorldServer", "Status: Testing");
					break;
				default:
					LogSuccess("WorldServer", "Status: Unknown");
					break;
			}
		}*/
	}
	else
	{
		LogWarning("WorldServer", "Unknow command : %s", str.c_str());
	}
	return false;
}

