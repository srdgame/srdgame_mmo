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
	int _account_id;
};
}

#endif
