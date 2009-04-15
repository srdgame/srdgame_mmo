#ifndef NPC_ID_H_
#define NPC_ID_H_

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
private:
	static int _cur_id;
};
}

#endif
