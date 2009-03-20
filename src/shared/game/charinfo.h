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

	std::string& get_name()
	{
		return _name;
	}
	Object* get_obj()
	{
		return _obj;
	}
	std::string _name;
	Object* _obj;
};

template <class T>
class CharInfoEx : public CharInfo
{
public:
	virtual ~CharInfoEx()
	{
		if(_ex)
			delete _ex;
	}

	T* get_ex_data()
	{
		return _ex;
	}
protected:
	T* _ex;
};
}

#endif
