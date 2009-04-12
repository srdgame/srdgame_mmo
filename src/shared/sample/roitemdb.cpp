#include "roitemdb.h"
#include "databasemgr.h"
#include "conversion_cast.h"
#include "queryresult.h"
#include "metadefs.h"
#include "log.h"

#include <cstring>
#include <string>

using namespace srdgame;
using namespace ro;
using namespace std;

#define RO_ITEM_DB_TB "item_db" // Now we only use the item_db.

void RoItemDB::init(short max_id)
{
	if (_inited)
		return;

	_inited = true;
	_max_id = max_id;
	_items = new RoDBItem[_max_id];
	
	// Do it for ourself.
	string sql = "SELECT `id`,`name_english`,`type`,`price_buy`,`price_sell`,`weight`,`attack`,`defence`,`range`,`slots`,`equip_jobs`,`equip_upper`,`equip_genders`,`equip_locations`,`weapon_level`,`equip_level`,`refineable`,`view` FROM `%s`";
	QueryResult* res = DatabaseMgr::get_singleton().query(sql.c_str(),
			RO_ITEM_DB_TB);
	if (!res)
	{
		LogError("RO_ITEM_DB", "Empty item db");
		return;
	}
	size_t count = res->get_row_size();
	size_t i = 0;
	for (; i < count; ++i)
	{
		Field* f = res->fetch();
		if (!fetch_item(f))
			break;
		res->next();
	}
	LogSuccess("RO_ITEM_DB", "%d items has been loaded!!", i);
	res->Delete();
	return;
}
/*
static void itemdb_jobid2mapid(unsigned int *bclass, unsigned int jobmask)
{
	int i;
	bclass[0]= bclass[1]= bclass[2]= 0;
	//Base classes
	if (jobmask & 1<<JOB_NOVICE)
	{	//Both Novice/Super-Novice are counted with the same ID
		bclass[0] |= 1<<MAPID_NOVICE;
		bclass[1] |= 1<<MAPID_NOVICE;
	}
	for (i = JOB_NOVICE+1; i <= JOB_THIEF; i++)
	{
		if (jobmask & 1<<i)
			bclass[0] |= 1<<(MAPID_NOVICE+i);
	}
	//2-1 classes
	if (jobmask & 1<<JOB_KNIGHT)
		bclass[1] |= 1<<MAPID_SWORDMAN;
	if (jobmask & 1<<JOB_PRIEST)
		bclass[1] |= 1<<MAPID_ACOLYTE;
	if (jobmask & 1<<JOB_WIZARD)
		bclass[1] |= 1<<MAPID_MAGE;
	if (jobmask & 1<<JOB_BLACKSMITH)
		bclass[1] |= 1<<MAPID_MERCHANT;
	if (jobmask & 1<<JOB_HUNTER)
		bclass[1] |= 1<<MAPID_ARCHER;
	if (jobmask & 1<<JOB_ASSASSIN)
		bclass[1] |= 1<<MAPID_THIEF;
	//2-2 classes
	if (jobmask & 1<<JOB_CRUSADER)
		bclass[2] |= 1<<MAPID_SWORDMAN;
	if (jobmask & 1<<JOB_MONK)
		bclass[2] |= 1<<MAPID_ACOLYTE;
	if (jobmask & 1<<JOB_SAGE)
		bclass[2] |= 1<<MAPID_MAGE;
	if (jobmask & 1<<JOB_ALCHEMIST)
		bclass[2] |= 1<<MAPID_MERCHANT;
	if (jobmask & 1<<JOB_BARD)
		bclass[2] |= 1<<MAPID_ARCHER;
//	Bard/Dancer share the same slot now.
//	if (jobmask & 1<<JOB_DANCER)
//		bclass[2] |= 1<<MAPID_ARCHER;
	if (jobmask & 1<<JOB_ROGUE)
		bclass[2] |= 1<<MAPID_THIEF;
	//Special classes that don't fit above.
	if (jobmask & 1<<21) //Taekwon boy
		bclass[0] |= 1<<MAPID_TAEKWON;
	if (jobmask & 1<<22) //Star Gladiator
		bclass[1] |= 1<<MAPID_TAEKWON;
	if (jobmask & 1<<23) //Soul Linker
		bclass[2] |= 1<<MAPID_TAEKWON;
	if (jobmask & 1<<JOB_GUNSLINGER)
		bclass[0] |= 1<<MAPID_GUNSLINGER;
	if (jobmask & 1<<JOB_NINJA)
		bclass[0] |= 1<<MAPID_NINJA;
}*/
bool RoItemDB::fetch_item(Field* f)
{
	try
	{
		/*`id`,`name_english`,`type`,`price_buy`,price_sell`,`weight`,`attack`,`defence`,`range`,`slots`,`equip_jobs`,`equip_upper`,`equip_genders`,`equip_locations`,`weapon_level`,`equip_level`,`refineable`,`view`*/ 
		size_t i = 0;
		uint16 id = f[i++].get<uint16>();
		if (_max_id < id)
			return true; // just don't load it.
		RoDBItem& item = *(_items + id);
		item._id = id;
		memcpy(_items[id]._name, f[i++].get<char*>(), MAX_ITEM_NAME_LEN);
		item._item_type = f[i++].get<int>();
		item._buy_val = f[i++].get<int>();
		item._sell_val = f[i++].get<int>();
		item._weight = f[i++].get<int>();
		item._atk = f[i++].get<int>();
		item._def = f[i++].get<int>();
		item._range = f[i++].get<int>();
		item._slots = f[i++].get<int>();
		item._equip_jobs = f[i++].get<unsigned int>();
		item._equip_upper = f[i++].get<unsigned int>();
		item._equip_sex = f[i++].get<int>();
		item._equip_point = f[i++].get<int>();
		item._wlv = f[i++].get<int>();
		item._elv = f[i++].get<int>();
		item._flag.no_refine = f[i++].get<int>() == 0 ? true : false;
		item._look = f[i++].get<int>();
	}
	catch (...)
	{
		return false;
	}
	return true;
}
