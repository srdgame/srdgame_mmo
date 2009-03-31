#ifndef _RO_DEFS_H_
#define _RO_DEFS_H_

//#include "defs.h"
#include "metadefs.h"

//using namespace srdgame;
// TODO: Replace these defines with configuration file.
#define DEFAULT_WALK_SPEED 150
#define MIN_WALK_SPEED 0
#define MAX_WALK_SPEED 1000
#define MAX_NAME_LEN 24
#define MAX_MAP_NAME_LEN 16
#define MAX_MAP_NAME_LEN_F 12
#define MAX_MSG_LEN 1024

#define SET_POS(p,pos,x,y,dir) \
	do { \
		uint8 *__p = (p); \
		__p+=(pos); \
		__p[0] = (uint8)((x)>>2); \
		__p[1] = (uint8)(((x)<<6) | (((y)>>4)&0x3f)); \
		__p[2] = (uint8)(((y)<<4) | ((dir)&0xf)); \
	} while(0)
// client-side: x0+=sx0*0.0625-0.5 and y0+=sy0*0.0625-0.5
#define SET_POS2(p,pos,x0,y0,x1,y1,sx0,sy0) \
	do { \
		uint8 *__p = (p); \
		__p+=(pos);	\
		__p[0]=(uint8)((x0)>>2); \
		__p[1]=(uint8)(((x0)<<6) | (((y0)>>4)&0x3f)); \
		__p[2]=(uint8)(((y0)<<4) | (((x1)>>6)&0x0f)); \
		__p[3]=(uint8)(((x1)<<2) | (((y1)>>8)&0x03)); \
		__p[4]=(uint8)(y1); \
		__p[5]=(uint8)(((sx0)<<4) | ((sy0)&0x0f)); \
	} while(0)

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
	char _map_name[16];
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

// For Map server
struct ConnectToMap
{
	uint32 _account_id;
	uint32 _char_id;
	uint32 _login_id1;
	uint32 _client_tick;
	uint8 _sex;
	// For version
	uint16 _ver;
};

struct MapAuthOK
{
	uint32 _tick_count;
	uint8 _x;
	uint8 _y;
	uint8 _dir;  // 人物朝向
};

struct WisMessage
{
	char _name[MAX_NAME_LEN];
	char _msg[MAX_MSG_LEN];
};

enum RoLookType
{
	LOOK_BASE,
	LOOK_HAIR,
	LOOK_WEAPON,
	LOOK_HEAD_BOTTOM,
	LOOK_HEAD_TOP,
	LOOK_HEAD_MID,
	LOOK_HAIR_COLOR,
	LOOK_CLOTHES_COLOR,
	LOOK_SHIELD,
	LOOK_SHOES,
};

struct RoLookUpdate
{
	RoLookType _type;
	uint32 _account_id;
	uint16 _id1;  // if weapon or shield, the _id1 is weapon, id2 is shield
	uint16 _id2;  // or id2 should be zero.
};

}
#endif
