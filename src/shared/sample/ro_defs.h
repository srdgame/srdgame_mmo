#ifndef _RO_DEFS_H_
#define _RO_DEFS_H_

//#include "defs.h"
#include "metadefs.h"
#include <vector>

/***************************************
 * Do not use std::string, please only use the char[]
 * ***************************************/

//using namespace srdgame;
//
namespace ro
{
// TODO: Replace these defines with configuration file.
#define DEFAULT_WALK_SPEED 150
#define MIN_WALK_SPEED 0
#define MAX_WALK_SPEED 1000
#define MAX_NAME_LEN 24
#define MAX_MAP_NAME_LEN 16
#define MAX_MAP_NAME_LEN_F 12
#define MAX_MSG_LEN 1024
#define MAX_ITEM_NAME_LEN 50

#define NPC_JOB_CLASS 45

static const uint16 MaxItemCount = 100; // TODO:
static const uint16 MaxCartCount = 100;
static const uint8 MaxSlotCount = 4;
static const uint16 MaxSkillCount = 1020;
static const uint16 MaxFriendCount = 40;
static const uint16 MaxHotKeyCount = 27;

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


//Equip position constants
enum equip_pos {
	EQP_HEAD_LOW = 0x0001, 
	EQP_HEAD_MID = 0x0200, //512
	EQP_HEAD_TOP = 0x0100, //256
	EQP_HAND_R   = 0x0002,
	EQP_HAND_L   = 0x0020, //32
	EQP_ARMOR    = 0x0010, //16
	EQP_SHOES    = 0x0040, //64
	EQP_GARMENT  = 0x0004,
	EQP_ACC_L    = 0x0008,
	EQP_ACC_R    = 0x0080, //128
	EQP_AMMO     = 0x8000, //32768
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
enum StatusP {
	SP_SPEED,SP_BASEEXP,SP_JOBEXP,SP_KARMA,SP_MANNER,SP_HP,SP_MAXHP,SP_SP,	// 0-7
	SP_MAXSP,SP_STATUSPOINT,SP_0a,SP_BASELEVEL,SP_SKILLPOINT,SP_STR,SP_AGI,SP_VIT,	// 8-15
	SP_INT,SP_DEX,SP_LUK,SP_CLASS,SP_ZENY,SP_SEX,SP_NEXTBASEEXP,SP_NEXTJOBEXP,	// 16-23
	SP_WEIGHT,SP_MAXWEIGHT,SP_1a,SP_1b,SP_1c,SP_1d,SP_1e,SP_1f,	// 24-31
	SP_USTR,SP_UAGI,SP_UVIT,SP_UINT,SP_UDEX,SP_ULUK,SP_26,SP_27,	// 32-39
	SP_28,SP_ATK1,SP_ATK2,SP_MATK1,SP_MATK2,SP_DEF1,SP_DEF2,SP_MDEF1,	// 40-47
	SP_MDEF2,SP_HIT,SP_FLEE1,SP_FLEE2,SP_CRITICAL,SP_ASPD,SP_36,SP_JOBLEVEL,	// 48-55
	SP_UPPER,SP_PARTNER,SP_CART,SP_FAME,SP_UNBREAKABLE,	//56-60
	SP_CARTINFO=99,	// 99

	SP_BASEJOB=119,	// 100+19 - celest
	SP_BASECLASS=120,	//Hmm.. why 100+19? I just use the next one... [Skotlex]

	// Mercenaries
	SP_MERCFLEE=165, SP_MERCKILLS=189, SP_MERCFAITH=190,
	
	// original 1000-
	SP_ATTACKRANGE=1000,	SP_ATKELE,SP_DEFELE,	// 1000-1002
	SP_CASTRATE, SP_MAXHPRATE, SP_MAXSPRATE, SP_SPRATE, // 1003-1006
	SP_ADDELE, SP_ADDRACE, SP_ADDSIZE, SP_SUBELE, SP_SUBRACE, // 1007-1011
	SP_ADDEFF, SP_RESEFF,	// 1012-1013
	SP_BASE_ATK,SP_ASPD_RATE,SP_HP_RECOV_RATE,SP_SP_RECOV_RATE,SP_SPEED_RATE, // 1014-1018
	SP_CRITICAL_DEF,SP_NEAR_ATK_DEF,SP_LONG_ATK_DEF, // 1019-1021
	SP_DOUBLE_RATE, SP_DOUBLE_ADD_RATE, SP_SKILL_HEAL, SP_MATK_RATE, // 1022-1025
	SP_IGNORE_DEF_ELE,SP_IGNORE_DEF_RACE, // 1026-1027
	SP_ATK_RATE,SP_SPEED_ADDRATE,SP_SP_REGEN_RATE, // 1028-1030
	SP_MAGIC_ATK_DEF,SP_MISC_ATK_DEF, // 1031-1032
	SP_IGNORE_MDEF_ELE,SP_IGNORE_MDEF_RACE, // 1033-1034
	SP_MAGIC_ADDELE,SP_MAGIC_ADDRACE,SP_MAGIC_ADDSIZE, // 1035-1037
	SP_PERFECT_HIT_RATE,SP_PERFECT_HIT_ADD_RATE,SP_CRITICAL_RATE,SP_GET_ZENY_NUM,SP_ADD_GET_ZENY_NUM, // 1038-1042
	SP_ADD_DAMAGE_CLASS,SP_ADD_MAGIC_DAMAGE_CLASS,SP_ADD_DEF_CLASS,SP_ADD_MDEF_CLASS, // 1043-1046
	SP_ADD_MONSTER_DROP_ITEM,SP_DEF_RATIO_ATK_ELE,SP_DEF_RATIO_ATK_RACE,SP_FREE3, // 1047-1050
	SP_HIT_RATE,SP_FLEE_RATE,SP_FLEE2_RATE,SP_DEF_RATE,SP_DEF2_RATE,SP_MDEF_RATE,SP_MDEF2_RATE, // 1051-1057
	SP_SPLASH_RANGE,SP_SPLASH_ADD_RANGE,SP_AUTOSPELL,SP_HP_DRAIN_RATE,SP_SP_DRAIN_RATE, // 1058-1062
	SP_SHORT_WEAPON_DAMAGE_RETURN,SP_LONG_WEAPON_DAMAGE_RETURN,SP_WEAPON_COMA_ELE,SP_WEAPON_COMA_RACE, // 1063-1066
	SP_ADDEFF2,SP_BREAK_WEAPON_RATE,SP_BREAK_ARMOR_RATE,SP_ADD_STEAL_RATE, // 1067-1070
	SP_MAGIC_DAMAGE_RETURN,SP_RANDOM_ATTACK_INCREASE,SP_ALL_STATS,SP_AGI_VIT,SP_AGI_DEX_STR,SP_PERFECT_HIDE, // 1071-1076
	SP_NO_KNOCKBACK,SP_CLASSCHANGE, // 1077-1078
	SP_HP_DRAIN_VALUE,SP_SP_DRAIN_VALUE, // 1079-1080
	SP_WEAPON_ATK,SP_WEAPON_ATK_RATE, // 1081-1082
	SP_DELAYRATE,SP_HP_DRAIN_RATE_RACE,SP_SP_DRAIN_RATE_RACE, // 1083-1085
	SP_IGNORE_MDEF_RATE,SP_IGNORE_DEF_RATE,SP_SKILL_HEAL2,SP_ADDEFF_ONSKILL, //1086-1089

	SP_RESTART_FULL_RECOVER=2000,SP_NO_CASTCANCEL,SP_NO_SIZEFIX,SP_NO_MAGIC_DAMAGE,SP_NO_WEAPON_DAMAGE,SP_NO_GEMSTONE, // 2000-2005
	SP_NO_CASTCANCEL2,SP_NO_MISC_DAMAGE,SP_UNBREAKABLE_WEAPON,SP_UNBREAKABLE_ARMOR, SP_UNBREAKABLE_HELM, // 2006-2010
	SP_UNBREAKABLE_SHIELD, SP_LONG_ATK_RATE, // 2011-2012

	SP_CRIT_ATK_RATE, SP_CRITICAL_ADDRACE, SP_NO_REGEN, SP_ADDEFF_WHENHIT, SP_AUTOSPELL_WHENHIT, // 2013-2017
	SP_SKILL_ATK, SP_UNSTRIPABLE, SP_AUTOSPELL_ONSKILL, // 2018-2020
	SP_SP_GAIN_VALUE, SP_HP_REGEN_RATE, SP_HP_LOSS_RATE, SP_ADDRACE2, SP_HP_GAIN_VALUE, // 2021-2025
	SP_SUBSIZE, SP_HP_DRAIN_VALUE_RACE, SP_ADD_ITEM_HEAL_RATE, SP_SP_DRAIN_VALUE_RACE, SP_EXP_ADDRACE,	// 2026-2030
	SP_SP_GAIN_RACE, SP_SUBRACE2, SP_FREE2,	// 2031-2033
	SP_UNSTRIPABLE_WEAPON,SP_UNSTRIPABLE_ARMOR,SP_UNSTRIPABLE_HELM,SP_UNSTRIPABLE_SHIELD,  // 2034-2037
	SP_INTRAVISION, SP_ADD_MONSTER_DROP_ITEMGROUP, SP_SP_LOSS_RATE, // 2038-2040
	SP_ADD_SKILL_BLOW, SP_SP_VANISH_RATE //2041
	//Before adding new bonuses, reuse the currently free slots:
	//2033 (SP_FREE2) (previously SP_ADDEFF_WHENHIT_SHORT)
	//1050 (SP_FREE3) (previously SP_ADD_SPEED)
};

#define EQP_WEAPON EQP_HAND_R
#define EQP_SHIELD EQP_HAND_L
#define EQP_ARMS (EQP_HAND_R|EQP_HAND_L)
#define EQP_HELM (EQP_HEAD_LOW|EQP_HEAD_MID|EQP_HEAD_TOP)
#define EQP_ACC (EQP_ACC_L|EQP_ACC_R)
enum item_types {
	IT_HEALING = 0,
	IT_UNKNOWN, //1
	IT_USABLE,  //2
	IT_ETC,     //3
	IT_WEAPON,  //4
	IT_ARMOR,   //5
	IT_CARD,    //6
	IT_PETEGG,  //7
	IT_PETARMOR,//8
	IT_UNKNOWN2,//9
	IT_AMMO,    //10
	IT_DELAYCONSUME,//11
	IT_MAX 
};

// define all the structs.
//
struct LoginInfo
{
	char name[MAX_NAME_LEN];
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
	uint32 _char_id;
	uint32 _ip;
	uint16 _port;
	char _map_name[MAX_MAP_NAME_LEN];
};
struct CharDataList
{
	int _count;
	struct RoCharInfoBase* _chars;
};
struct CreateCharData
{
	char _name[MAX_NAME_LEN];
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
	char _new_name[MAX_NAME_LEN];
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
struct RoMessage
{
	size_t _len;
	int _id; // 0 to have no id.
	const char* _msg;
};


struct RoLookUpdate
{
	RoLookType _type;
	uint32 _account_id;
	uint16 _id1;  // if weapon or shield, the _id1 is weapon, id2 is shield
	uint16 _id2;  // or id2 should be zero.
};
struct RoCharFriendInfo;
struct RoFriendList
{
	std::vector<RoCharFriendInfo>* _friends;
};
struct RoFriendStatus
{
	int _account_id;
	int _char_id;
	bool _online;
};
//class RoCharItem;
struct RoItemListData
{
	int _id; // Item id
	int _type; // Item type
	int _identify;
	int _equiped_point;
	int _attrs;
	int _refine;
	//RoCharItem* _info;
	int _equip_point; // This is all the equipable locations.
	int _amount;
};
struct RoItemData : public RoItemListData
{
	int _index;
	int _fail; // 0 is ok.
};

struct RoItemList
{
	int _start_index;
	std::vector<RoItemListData> * _items;
};


struct RoPoint
{
	short _x;
	short _y;
};
union RoWalkToXY
{
	RoPoint _point;
	int _int;
};
struct RoWalkToXY_OK
{
	int _tick;
	RoPoint _org;
	RoPoint _to;
};

struct RoUnitMove
{
	int _tick;
	int _id;
	RoPoint _org;
	RoPoint _to;
};

struct RoEquipItem
{
	short _index;
	short _pos;
};
union RoEquipItemUnion
{
	RoEquipItem _item;
	long _long;
};
struct RoEquipItemOK
{
	short _index;
	short _pos;
	int _ok;
};
typedef RoEquipItemOK RoUnequipItemOK;

struct RoStatusUpdate_int // 0x00b0 0x00b1
{
	StatusP _status;
	int _val;
};
struct RoStatusUpdate_byte // 0x00be
{
	StatusP _status;
	char _val;
};
struct RoAtAckRangeUpdate // 0x013a
{
	short _range;
};
struct RoStatusUpdate_int_int // 0x0141
{
	StatusP _status;
	int _val;
	int _val2;
};
struct RoCartInfoUpdate // 0x0121
{
	short _cart_num;
	short _max_cart; // it is predefine.
	int _cart_weight;
	int _battle_max_cart_weight;
};
struct RoMannerChangeUpdate // 0x01ab
{
	int _id; // the account_id when it is used for player.
	StatusP _status; // == SP_MANNER
	int _val;
};

enum RoActionType
{
	AT_DAMAGE = 0x00, // Damage
	AT_PICK_UP_ITEM = 0x01, // Pick up item.
	AT_SIT_DOWN = 0x02, // sit down
	AT_STAND_UP = 0x03, // stand up
	AT_RA_DAMAGE = 0x04, // reflected/absorbed damage.
	AT_DOUBLE_ATTACK = 0x08, // double attack
	AT_NO_FLINCH = 0x09, // don't display flinch animation
	AT_CRITICAL_HIT = 0x0a,  // critical hit
	AT_LUCKY_DODGE = 0x0b, // locky dodge.
};
struct RoActionData
{
	int _src_id;
	int _dst_id;
	int _tick_count;
	int _src_delay;
	int _dst_delay;
	int _div;
	RoActionType _type;
	int _damage;
	int _damage2;
};
enum DisappearType
{
	DT_DIED = 0x01, // died
	DT_RESPAWN = 0x02, // respawned
	DT_GONE = 0x03, // teleported / logged out
};
struct RoUnitDisappear
{
	int _id;
	DisappearType _type;
};
enum GuildRequestInfoType
{
	GRIT_BASIC_INFO_AND_ALLIANCE_INFO = 0,
	GRIT_POSITION_NAME_LIST_AND_MEMBER_LIST = 1,
	GRIT_POSITTON_NAME_LIST_AND_POSITION_INFO_LIST = 2,
	GRIT_SKILL_INFO = 3,
	GRIT_EXPULSION_LIST = 4,
};
struct RoRequestCharName
{
	int _id;
	char _name[MAX_NAME_LEN]; // 6
};
struct RoRequestCharNameEx : public RoRequestCharName
{
	char _name_1[MAX_NAME_LEN]; // 30
	char _name_2[MAX_NAME_LEN];// 54 -
	char _name_3[MAX_NAME_LEN];// 78
};
struct RoChangeDir
{
	char _head_dir;
	char _dir;
};
union RoChangeDirUnion
{
	long _long;
	RoChangeDir _dir;
};
struct RoChangedDir
{
	int _id; // account_id;
	int _head_dir; // 0 if it is not player.
	int _dir; // dir;
};
struct RoNpcBuySellSelect
{
	int _npc_id;
	unsigned char _flag;
};
struct RoSellingItem
{
	int _val; // The value from DB
	int _s_val; // The value for current player, it may cheaper if player has the skill.
	int _item_type; // item type.
	int _item_id; // The id in DB
};
struct RoSellingItemList
{
	int _count;
	RoSellingItem* _items;
};
struct RoItemValue
{
	int _index; // The index in item list of Player
	int _val;
	int _s_val;
};
struct RoItemValueList
{
	int _count;
	RoItemValue* _items;
};
}
#endif
