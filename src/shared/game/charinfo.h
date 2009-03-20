#ifndef CHAR_INFO_H_
#define CHAR_INFO_H_
#include <string>

namespace srdgame
{
class Object;
class CharInfo
{
public:
	virtual ~CharInfo(){}

	inline int get_class()
	{
		return _class;
	}
	inline std::string& get_name()
	{
		return _name;
	}
protected:
	int _class;// character classes.
	// TODO:
	std::string _name;
};

template <class T>
class CharInfoEx : public CharInfo
{
public:
	CharInfoEx(T& t) : _ex(t)
	{
	}
	virtual ~CharInfoEx()

	inline T* get_ex_data()
	{
		return &_ex;
	}
protected:
	T _ex;
};
}

#endif
