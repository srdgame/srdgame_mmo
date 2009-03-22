#ifndef CHAR_INFO_H_
#define CHAR_INFO_H_
#include <string>
#include "npcinfo.h"

namespace srdgame
{
struct CharExp : public NpcExp
{
	CharExp() : _prop_point (0), _skill_point(0)
	{
	}
	int _prop_point;
	int _skill_point;

};
struct CharInfo : public NpcInfo
{
	CharInfo() : _account_id(0)
	{
	}
	int _account_id;
};
}

#endif
