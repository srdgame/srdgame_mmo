#ifndef FRIEND_H_
#define FRIEND_H_

#include <string>

struct FriendInfo
{
	FriendInfo() : _account_id(0), _char_id(0)
	{
	}
	int _account_id;
	int _char_id;
	std::string _name;
	std::string _nick_name;
};

#endif
