#ifndef SKILL_H_
#define SKILL_H_
namespace srdgame
{
struct SkillInfo
{
	uint32 _id;
	uint16 _lvl;
	uint16 _flag;
};

template <class T>
class SkillEx : public SkillInfo
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
