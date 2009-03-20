#ifndef PARTY_H_
#define PARTY_H_

namespace srdgame
{
class Party
{
public:
	Party(uint32 id) : _id(id)
	{
	}
	virtual ~Party()
	{
	}

	inline uint32 get_owner()
	{
		return _owner_id;
	}
	inline size_t get_member_count()
	{
		return _members.size();
	}
	inline std::string& get_name()
	{
		return _name;
	}

proteceted:
	static const _max_member_count = 32;
	uint32 _id;
	uint32 _type;
	uint32 _owner_id;
	std::string _name;
	std::vector<uint32> _members;
};

template <class T>
class PartyEx : public Party
{
public:
	PartyEx(T& t, uint32 id) : Party(id), _ex(t)
	{
	}
	virtual PartyEx()
	{
	}
	inline T* get_ex_data()
	{
		return &_ex;
	}

protected:
	T _ex;
};
}

#endif
