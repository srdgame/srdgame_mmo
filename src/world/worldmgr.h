// This will mention all the data of world 

#ifndef LOGIN_MANAGER_H_
#define LOGIN_MANAGER_H_

#include "singleton.h"
#include <vector>
#include <map>

#include "socketdefs.h"
#include "mutex.h"
#include "typedefs.h"
#include "rosql.h"

namespace srdgame
{
class Player;
class Updater;
class WorldMgr : public Singleton < WorldMgr >
{
public:
	WorldMgr();
	virtual ~WorldMgr();

	void init();

	void enum_world_servers(std::vector<WorldSrvInfo>& info);

	void add_client(Player* s);
	void remove_client(Player* s);

	// world server attributes.
	void set_name(std::string name);
	std::string get_name();
	void set_info(WorldSrvInfo& info);
	WorldSrvInfo& get_info();

	Updater* get_updater();
protected:
	bool setup_player(Player* s);

	bool reg_to_map(Player* p);
	
	void game_tips(Player* s);

protected:
	// char_id and player object.
	std::map<int, Player*> _clients;
	Mutex _client_lock;

	Mutex _lock;
	bool _inited;
	WorldSrvInfo _info;
	Updater* _updater;

	// SQL, no need lock since this is no data contains 
	ro::RoSql* _sql;
};

}


#endif
