// This will mention all the data of world 

#ifndef LOGIN_MANAGER_H_
#define LOGIN_MANAGER_H_

#include "singleton.h"
#include <vector>
#include <map>

#include "socketdefs.h"
#include "mutex.h"
#include "typedefs.h"

namespace srdgame
{
class WorldSocketBase;
class WorldMgr : public Singleton < WorldMgr >
{
public:
	WorldMgr();
	virtual ~WorldMgr();

	void enum_world_servers(std::vector<WorldSrvInfo>& info);

	void add_client(WorldSocketBase* s);
	void remove_client(WorldSocketBase* s);

	// world server attributes.
	void set_name(std::string name);
	std::string get_name();
	void set_info(std::string info);
	std::string get_info();

protected:
	std::map<SOCKET, WorldSocketBase*> _clients;
	Mutex _server_lock;
	Mutex _client_lock;

	Mutex _lock;
	std::string _name;
	std::string _info;
};

}


#endif
