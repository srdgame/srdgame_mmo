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
struct SkillEx : public SkillInfo
{
	T _ex;
};
}
#endif
