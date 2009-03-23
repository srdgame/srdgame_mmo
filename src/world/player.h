#ifndef PLAYER_H_
#define PLAYER_H_

//#include "charinfo.h"

namespace srdgame
{
struct CharInfo;
class Map;
class Player
{
public:
	virtual ~Player();
	virtual CharInfo* get_info() = 0;
	virtual bool set_map(Map* map) = 0;
	virtual int get_id() = 0;
};
}

#endif
