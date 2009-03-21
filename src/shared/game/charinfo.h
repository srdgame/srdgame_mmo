#ifndef CHAR_INFO_H_
#define CHAR_INFO_H_
#include <string>
#include "npcinfo.h"

namespace srdgame
{
struct CharExp : public NpcExp
{
	int _prop_point;
	int _skill_point;

};
struct CharInfo : public NpcInfo
{
public:
	CharInfo(int account_id) : _account_id(account_id)
	{
	}
	virtual ~CharInfo()
	{
	}
	inline int get_account_id()
	{
		return _account_id;
	}
protected:
	int _account_id;
};
}

#endif
