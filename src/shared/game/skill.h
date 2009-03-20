#ifndef SKILL_H_
#define SKILL_H_
namespace srdgame
{
class Skill
{
public:
	Skill(uint32 id, uint16 lvl = 0, uint16 flag = 0) :
		_id(id), _lvl(lvl), _flag(flag)
	{
	}
	virtual ~Skill(){}

	inline uint32 get_id()
	{
		return _id
	}
	inline uint16 get_lvl()
	{
		return _lvl;
	}
	inline void set_lvl(uint16 lvl)
	{
		_lvl = lvl;
	}
	inline uint16 get_flag()
	{
		return _flag;
	}
	inline void set_flag(uint16 flag)
	{
		_flag = flag;
	}
protected:
	uint32 _id;
	uint16 _lvl;
	uint16 _flag;
};

template <class T>
class SkillEx : public Skill
{
	SkillEx(T& t, uint32 id, uint16 lvl = 0, uint16 flag = 0) :
		Skill(id, lvl, flag), _ex(t)
	{
	}
	virtual ~SkillEx()
	{
	}
	inline T* get_ex_data()
	{
		return _ex;
	}
protected:
	T _ex;
};
}
#endif
