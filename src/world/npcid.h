#ifndef NPC_ID_H_
#define NPC_ID_H_

#define MIN_NPC_ID 100000000;

namespace srdgame
{
class NpcId
{
public:
	static inline int generate()
	{
		// We may need lock later, but so far this is should be fine.
		return ++_cur_id;
	}
	static inline bool is_npc(int id)
	{
		return id > MIN_NPC_ID;
	}
private:
	static int _cur_id;
};
}

#endif
