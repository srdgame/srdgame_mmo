#ifndef PARTY_H_
#define PARTY_H_

namespace srdgame
{
struct Party
{
	static const _max_member_count = 32;
	uint32 _id;
	uint32 _type;
	uint32 _owner_id;
	std::string _name;
	std::vector<uint32> _members;
};

template <class T>
struct PartyEx : public Party
{
	T _ex;
};
}

#endif
