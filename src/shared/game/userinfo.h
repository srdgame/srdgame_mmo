#ifndef _USER_INFO_H_
#define _USER_INFO_H_

#include <string>
#include "metadefs.h"

namespace srdgame
{
struct UserInfo
{
	long _id;
	std::string _name; // user name;
	std::string _pass; // password;
	long _crypto; // crypto type.
};

template <class T>
struct UserInfoEx : public UserInfo
{
	T _ex;
};

}
#endif
