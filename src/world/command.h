#ifndef COMMAND_H_
#define COMMAND_H_

#include "singleton.h"
#include <vector>
#include <string>

namespace srdgame
{
class Player;
class Map;
class Command : public Singleton < Command >
{
	enum CommandType
	{
		CT_NONE,
		CT_SPAWN_MOB,
		CT_ADD_ITEM,
		CT_JOB_LEVEL,
		CT_BASE_LEVEL,
		CT_CMD_COUNT,
	};
	static const std::string CMDs[CT_CMD_COUNT];
	struct CMD
	{
		CMD() : _type(CT_NONE){}
		CommandType _type;
		std::vector<std::string> _params;
	};
public:
	Command();
	~Command();
	
	void handle(const Player* p, const char* msg);

private:
	void spawn_mob(const Map* map, int x, int y, int mob_id);
	void spawn_mob(const Map* map, int x, int y, const std::string& name);
	void spawn_npc(const Map* map, int x, int y, int class_id, const std::string& name);
	void add_item(const Player* p, int item_id, int quantity);
	void add_item(const Player* p, const std::string& name, int quantity);
};
};

#endif
