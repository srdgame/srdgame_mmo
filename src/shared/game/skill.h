#ifndef SKILL_H_
#define SKILL_H_
namespace srdgame
{
struct SkillInfo
{
	SkillInfo() : _id(0), _lvl(0), _flag(0)
	{
	}
	int _id;
	int _lvl;
	int _flag;
};

template <class T>
struct SkillInfoEx : public SkillInfo
{
	T _ex;
};
}
#endif
