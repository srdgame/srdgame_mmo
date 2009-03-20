#ifndef _USER_INFO_H_
#define _USER_INFO_H_

#include <string>
#include "metadefs.h"

namespace srdgame
{
class UserInfo
{
public:
	UserInfo() : _id(0), _crypto(0)
	{
	}
	virtual ~UserInfo()
	{
	}
public:
	long _id;
	std::string _name; // user name;
	std::string _pass; // password;
	long _crypto; // crypto type.
};

template <class T>
class UserInfoEx : public UserInfo
{
public:
	UserInfoEx(T* data) : UserInfo(), _ex_data(*data)
	{
	}
	UserInfoEx() : UserInfo()
	{
	}
	~UserInfoEx()
	{
	}

	T* get_ex_data()
	{
		return &_ex_data;
	}
private:
	T _ex_data;
};

}
#endif
