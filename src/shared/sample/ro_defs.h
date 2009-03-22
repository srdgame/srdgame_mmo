#ifndef _RO_DEFS_H_
#define _RO_DEFS_H_

//#include "defs.h"
#include "metadefs.h"

//using namespace srdgame;

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

// Char server info;
struct ServerInfo
{
	uint32 ip;
	uint16 port;
	char name[20];
	uint16 users;
	uint16 maintenance;
	uint16 new_;
};
// Char server list.
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

// For Char servers.
//
struct LoginToChar
{
	uint32 _account;
	uint32 _id1;
	uint32 _id2;
	uint8 _sex;
	bool _auth;
};
struct MapServerInfo
{
	uint32 _account;
	uint32 _ip;
	uint16 _port;
	char _name[16];
};
struct CharDataList
{
	int _count;
	struct RoCharInfo* _chars;
};
struct CreateCharData
{
	char _name[24];
	uint8 _str;
	uint8 _agi;
	uint8 _vit;
	uint8 _int;
	uint8 _dex;
	uint8 _luk;
	uint8 _slot; // The slot index for select characters.... -_-!
	uint16 _hair_color;
	uint16 _hair_style;
};
struct DeleteCharData
{
	uint32 _id;
	char _email[40];
	// MORE?
};
struct CharRenameData
{
	uint32 _id;
	uint32 _char_id;
	char _new_name[24];
};
// TODO: Replace these defines with configuration file.
#define DEFAULT_WALK_SPEED 150
#define MIN_WALK_SPEED 0
#define MAX_WALK_SPEED 1000
#define MAX_NAME_LEN 24
#define MAX_MAP_NAME_LEN 16
}
#endif
