#include "realmserver.h"
#include "configfile.h"
#include "log.h"
#include "network.h"
#include "threadpool.h"
#include <iostream>
#include <vector>
#include "typedefs.h"
#include "realmmgr.h"
#include "realmsocketl.h"
#include "packetparser.h"

using namespace srdgame;

RealmServer::RealmServer(const std::string& conf_fn) : _conf_fn(conf_fn), _config(NULL), _socket(NULL), _inter_socket(NULL), _login_socket(NULL)
{
//	LogDebug("RealmServer", "Create the server with connfigure file:%s", conf_fn.c_str());
}

RealmServer::~RealmServer()
{
	//LogDebug("RealmServer", "Destructor of RealmServer");
	if (_config)
	{
	//	LogDebug("RealmServer", "Deleting config");
		delete _config;
		_config = NULL;
	//	LogDebug("RealmServer", "Deleted config");
	}
	//std::string input;
	//std::cin >> input;
	//LogDebug("RealmServer", "Stoping sockets");
	if (_socket)
		delete _socket;
	if (_inter_socket)
		delete _inter_socket;
}

void RealmServer::run()
{
	if (!this->load_conf())
	{
		LogError("RealmServer", "Could not load configure file: %s", _conf_fn.c_str());
		return;
	}
	LogDebug("RealmServer", "Load configuration file completed");
	if (!this->init_packet_parser())
	{
		LogError("WorldServer", "Could not load protocol so file");
		return;
	}

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
	if (!this->connect_login())
	{
		LogError("RealmServer", "Could not connect to login server");
	}
	LogNotice(NULL, "-------------------------------------------------");
	while(!wait_command())
	{
		LogNotice(NULL, "-----------------------");
	}
}
void RealmServer::lost_realm()
{
	_login_socket = NULL;
}
bool RealmServer::init_env()
{
	ThreadPool::get_singleton().init(10);
	SocketMgr::get_singleton().start_worker();
	RealmMgr::get_singleton().set_name(_config->get_value<std::string>("NAME"));
	RealmMgr::get_singleton().set_info(_config->get_value<std::string>("INFO"));
	return true;
}
bool RealmServer::init_packet_parser()
{
	return PacketParser::get_singleton().init_ex(_config->get_value<std::string>("EX_PROTOCOL_LIB").c_str());
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
	if (port <= 0)
	{
		LogWarning("RealmServer", "Invalid listen port found, change it to: %d", 7001);
		port = 7001;
	}
	std::string addr = _config->get_value<std::string>("ADDRESS");
	if (addr.empty())
	{
		addr = "127.0.0.1";
		LogWarning("RealmServer", "Invalid listen address has found");
	}

	LogDebug("RealmServer", "Try to listen on: %s %d", addr.c_str(), port);
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
	if (port <= 0)
	{
		port = 7101;
		LogWarning("RealmServer", "Inavlid inter port has found, change it to: %d", port);
	}
	std::string addr = _config->get_value<std::string>("INTER_ADDRESS");
	if (addr.empty())
	{
		addr = "127.0.0.1";
		LogWarning("RealmServer", "Invalid inter address has found");
	}
	LogDebug("RealmServer", "Try to inter listen on: %s %d", addr.c_str(), port);
	_inter_socket = new TcpListenSocket<RealmInterSocketW>(addr.c_str(), port);
	if (_inter_socket->is_open())
	{
		LogSuccess("RealmServer", "Register listening on port: %d", port);
		return true;
	}
	return false;
}

bool RealmServer::connect_login()
{
	int has_realm = _config->get_value<int>("ENABLE_REALM");
	if (has_realm == 0)
	{
		// There is no realm server.
		return true;
	}
	int port = _config->get_value<int>("REALM_PORT");
	if (0 >= port)
	{
		port = 6101;
		LogWarning("RealmServer", "Invalid realm server port has found, adjust to: %d", port);
	}
	std::string addr = _config->get_value<std::string>("REALM_ADDRESS");
	if (addr.empty())
	{
		addr = "127.0.0.1";
		LogWarning("RealmServer", "Can not found realm server address, use default: %s", addr.c_str());
	}
	LogDebug("RealmServer", "Try to connect to realm server : %s %d", addr.c_str(), port);
	if (_login_socket)
	{
		_login_socket->close();
	}
	_login_socket = new RealmInterSocketL(this);
	if (!_login_socket->connect(addr, port))
	{
		LogDebug("RealmServer", "Could not connect to realm server, please start realm server first or correct your configuration file");
		return false;
	}
	
	Packet p;
	p.op = I_NOTIFY;
	p.len = sizeof(Packet);
	LogDebug("RealmServer", "sizeof(Packet) is %d", sizeof(Packet));
	p.param.Long = 1;
	_login_socket->send_packet(&p);
	//_login_socket->send_packet(&p);
	
	return true;
}


bool RealmServer::wait_command()
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
		std::vector<WorldSrvInfo> info;
		RealmMgr::get_singleton().enum_world_servers(info);
		size_t i = 0;
		if (0 == info.size())
		{
			LogSuccess("RealmServer", "No world server avaliable yet!");
		}
		for (; i < info.size(); ++i)
		{
			LogSuccess("RealmServer", "World Server : %d", i);
			LogSuccess("RealmServer", "Name: %s\t Info: %s", info[i].name.c_str(), info[i].info.c_str());
			switch (info[i].type)
			{
				case WT_NORMAL:
					LogSuccess("RealmServer", "Type: Normal Server");
					break;
				case WT_PVP:
					LogSuccess("RealmServer", "Type: PVP Server");
					break;
				case WT_TESTING:
					LogSuccess("RealmServer", "Type: Testing Server");
					break;
				default:
					LogWarning("RealmServer", "Type: Unknown");
					break;
			}

			switch (info[i].status)
			{
				case WS_OFFLINE:
					LogSuccess("RealmServer", "Status: Offline");
					break;
				case WS_READY:
					LogSuccess("RealmServer", "Status: Ready");
					break;
				case WS_FULL:
					LogSuccess("RealmServer", "Status: Closing");
					break;
				case WS_TESTING:
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

