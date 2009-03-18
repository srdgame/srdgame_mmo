#ifndef RO_USER_H_
#define RO_USER_H_

#include "metadefs.h"

namespace ro
{
struct RoUser
{
	//uint32 id;
	//char name[24];
	//char pass[32];
	char _email[40];
	char _sex;
	uint8 _level;
	uint16 _login_count;
	uint32 _state;
	uint32 _unban_time;
	uint32 _expire_time;
	char _last_login[20]; // DateTime.
	char _last_ip[100]; //
};
}

#endif
