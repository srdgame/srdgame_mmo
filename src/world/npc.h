#ifndef NPC_H_
#define NPC_H_

namespace srdgame
{
struct NpcInfo;
class Npc
{
public:
	virtual ~Npc();
	virtual NpcInfo* get_info() = 0;
	virtual int get_id() = 0;
};
}

#endif
