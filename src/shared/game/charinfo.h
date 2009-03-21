#ifndef CHAR_INFO_H_
#define CHAR_INFO_H_
#include <string>

namespace srdgame
{
class Object;
struct CharInfo
{
	int _class;// character classes.
	// TODO:
	std::string _name;
};

template <class T>
struct CharInfoEx : public CharInfo
{

	T _ex;
};
}

#endif
