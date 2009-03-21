#ifndef PARTY_H_
#define PARTY_H_

#include <string>
#include <vector>

namespace srdgame
{
struct PartyInfo
{
	int _id;
	int _type;
	int _owner_id;
	std::string _name;
	std::vector<int> _members;
	static const int _max_member_count = 32;
};

template <class T>
struct PartyInfoEx : public PartyInfo
{
	T _ex;
};
}

#endif
