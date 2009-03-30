#ifndef RO_MAP_H_
#define RO_MAP_H_

#include <string>
#include "metadefs.h"
#include "ro_defs.h"
#include <fstream>
#include <vector>

namespace ro
{
struct MapCacheMainHeader
{
	uint32 _file_size;
	uint16 _map_count;
};
struct MapCacheMapInfo
{
	char _name[MAX_MAP_NAME_LEN_F];
	int16 _xs;
	int16 _ys;
	int32 _len;
};
struct MapCell
{
	// terrain flags
	unsigned char
		walkable : 1,
		shootable : 1,
		water : 1;

	// dynamic flags
	unsigned char
		npc : 1,
		basilica : 1,
		landprotector : 1,
		novending : 1,
		nochat : 1;

#ifdef CELL_NOSTACK
	unsigned char cell_bl; //Holds amount of bls in this cell.
#endif
};
struct MapData
{
	char _name[MAX_MAP_NAME_LEN_F];
	uint16 _index;
	MapCell* _cells;
	uint16 _xs;
	uint16 _ys;
	struct MapFlag {
		unsigned _town : 1;
		unsigned _auto_trade : 1;
		unsigned _allow_ks : 1;
		unsigned _no_memo : 1;
		unsigned _no_teleport : 1;
		unsigned _no_return : 1;
		unsigned _monster_no_teleport : 1;
		unsigned _no_save : 1;
		unsigned _no_branch : 1;
		unsigned _no_exp_penalty : 1;
		// PVP & GVG
		unsigned _pvp : 1;
		unsigned _pvp_no_party : 1;
		unsigned _pvp_no_guild : 1;
		unsigned _pvp_nightmare_drop : 1;
		unsigned _pvp_no_cal_crank : 1;
		unsigned _gvg_castle : 1;
		unsigned _gvg : 1;
		unsigned _gvg_dungeon : 1;
		unsigned _gvg_no_party : 1;
		unsigned _battle_ground : 2;
		// Limit
		unsigned _no_zeny_penalty : 1;
		unsigned _no_trade : 1;
		unsigned _no_skill : 1;
		unsigned _no_warp : 1;
		unsigned _no_warp_to : 1;
		unsigned _no_icewall : 1;
		// Weather.
		unsigned _snow : 1;
		unsigned _clouds : 1;
		unsigned _coulds2 : 1;
		unsigned _fog : 1;
		unsigned _fireworks : 1;
		unsigned _sakura : 1;
		unsigned _leaves : 1;
		unsigned _rain : 1;
		// Limit
		unsigned _in_doors : 1;
		unsigned _no_go : 1;
		unsigned _no_base_exp : 1;
		unsigned _no_job_exp : 1;
		unsigned _no_mob_loot : 1;
		unsigned _no_mvp_loot : 1;
		unsigned _night_enabled : 1;
		unsigned _restricted : 1;
		unsigned _no_drop : 1;
		unsigned _no_vending : 1;
		unsigned _load_event : 1;
		unsigned _no_chat : 1;
		unsigned _party_lock : 1;
		unsigned _guild_lock : 1;

		// block commands for non-gms
		unsigned _no_command : 1;
	} _flag;
	int _zone;
	int _job_exp;
	int _base_exp;
};

class RoMap
{
public:
	RoMap();
	virtual ~RoMap();

	bool load(const char* fn);
	void unload();

	int get_map(const char* name, MapData& data);

	inline bool is_loaded()
	{
		return _loaded;
	}

protected:
	MapCacheMainHeader _header;
	std::vector<MapCacheMapInfo> _maps;
	std::fstream _file;
	bool _loaded;
};
}

#endif
