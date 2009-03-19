#ifndef _RO_DEFS_H_
#define _RO_DEFS_H_

#include "defs.h"

namespace ro
{
// define all the structs.
//
struct LoginInfo
{
	char name[24];
	char pass[32];
	uint8 crypto_type;
	uint8 client_type;
	uint32 version;
};
struct AdminLoginInfo
{
	uint16 crypto_type;
	char pass[32];
};
struct ServerInfo
{
	uint32 ip;
	uint16 port;
	char name[20];
	uint16 users;
	uint16 maintenance;
	uint16 new_;
};
struct ServerListHeader
{
	uint32 account;
	uint32 id1;
	uint32 id2;
	uint8 sex;
	uint16 server_count;
	uint32 ip; //which ip?
};
struct RO_MD5Key
{
	char key[16]; // use max one.
};
struct LoginServerInfo
{
	char info[8];
};
}
#endif
