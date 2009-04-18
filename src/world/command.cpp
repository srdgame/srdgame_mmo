#include "command.h"
#include "player.h"
#include "map.h"
#include "conversion_cast.h"
#include "roitemdb.h"
#include "itemhelper.h"
#include "log.h"

using namespace std;
using namespace srdgame;
using namespace ro;

const string Command::CMDs[CT_CMD_COUNT] = 
{
	"none",
	"spawn_mob",
	"add_item",
	"job_lvl",
	"base_lvl",
};
Command::Command()
{
}
Command::~Command()
{
}

void Command::handle(const Player* p, const char* msg)
{
	if (*msg != '.')
		return;
	msg++;

	int i = 0;
	for (; i < CT_CMD_COUNT; ++i)
	{
		if (0 == strncmp(msg, CMDs[i].c_str(), CMDs[i].length()))
		{
			msg += CMDs[i].length();
			break;
		}
	}
	if (i == CT_CMD_COUNT)
	{
		return;
	}
	while (*msg == ' ')
	{
		msg++;
	}
	switch (i)
	{
		case CT_NONE:
			break;
		case CT_SPAWN_MOB:
			break;
		case CT_ADD_ITEM:
			{
				char name[MAX_ITEM_NAME_LEN];
				memset(name, 0, sizeof(char) * MAX_ITEM_NAME_LEN);
				int index = 0;
				bool num = true;
				while (*(msg + index) != '[' && index < MAX_ITEM_NAME_LEN)
				{
					name[index] = *(msg + index);
					if (num)
					{
						if (name[index] > '9' || name[index] < '0')
							num = false;
					}
					index++;
				}
				index++;
				string q(msg + index);
				int quantity = conversion_cast<int>(q);

				if (num)
				{
					string num (name);
					add_item(p, conversion_cast<int>(num), quantity);
				}
				else
				{
					add_item(p, string(name), quantity);
				}
			}
			break;
		default:
			break;
	}

	return;
}

void Command::spawn_mob(const Map* map, int x, int y, int mob_id)
{
}
void Command::spawn_mob(const Map* map, int x, int y, const string& name)
{
}
void Command::add_item(const Player* p, int item_id, int quantity)
{
	if (item_id > RoItemDB::get_singleton().get_max_id())
		return;

	LogError("Command", "Adding Item : id - %d \t quantity - %d", item_id, quantity);
	const RoDBItem* db = RoItemDB::get_singleton().get_item(item_id);
	if (db)
	{
		if (!isstackable2(db))
		{
			// TODO:
			RoCharItem item;
			item._type = db->_id;
			item._identify = 1;
			item._refine = quantity < 15 ? quantity : 15;
			item._amount = 1;
			((Player*)p)->add_item(item);
		}
		else
		{
			RoCharItem item;
			item._type = db->_id;
			item._identify = 1;
			item._refine = 0;
			item._amount = quantity < 99 ? quantity : 99;
			((Player*)p)->add_item(item);
		}
	}
	else
	{
		//TODO:
	}
}
void Command::add_item(const Player* p, const string& name, int quantity)
{
}
