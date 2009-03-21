#ifndef SKILL_H_
#define SKILL_H_
namespace srdgame
{
struct SkillInfo
{
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
