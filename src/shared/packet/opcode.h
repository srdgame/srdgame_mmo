#ifndef _OPCODE_H_
#define _OPCODE_H_

namespace srdgame
{
namespace opcode
{
/*
 * All the last member with number is the Max number but not must number
 * It means you can provide less info only for last member, without any warry
 * Only the one I have comment for you that you could not provide less data.
 *
 */
struct InitData
{
	long _flag;
}
struct InitResult
{
	long _flag;
}
struct LoginData
{
	char _username[128];
	char _password[128]; // Not enough?
	char _other[128];
};
struct LoginResult
{
	long _flag; // 0 = OK, 1 = Refect, 2 = Incorrect PWD, 3 = Block list, 4 = Not latest client, 5 = Account Expired, 6 = Banned, 7 = Server too busy, 8 = Account Limitition, 9 = Self Lock, 10 = Not permitted Group, 11 = Account erased, 12 = Hacking... 13 = Unknow for sure :-)
	char _info[256]; // more detial info
};
struct LogoutData
{
	long _flag;
	char _other[256]; // 
};
struct LogoutResult
{
	long _flag; //
	char _info[256];
};

struct ServerList
{
	long _type;
	char _other[256];
};
struct ServerInfo
{
	long _type;
	long _id;
	long _ip;
	long _port;
	char _name[128];
	char _info[128];
	char _other[256]; // This is must be 256 size.
};
struct ServerListResult
{
	long _count;
	struct ServerInfo _list[];
	char _other[256];
};

struct EnterServerResult
{
	long _flag;
	char _other[256];
};

struct CryptoKeyData
{
	char _key[256];
	char _other[256];
}
struct CryptoKeyResult
{
	char _key[256];
	char _other[256];
}

enum ExOpcode
{
	// ES_*** send out by server only
	// EC_*** send out by game client only.
	// Every item should marked with exactly number.
	//
	EC_NONE = 0, // For nopacket( :-) just the packet we won't handle it);
	EC_INIT = 1,
	ES_INIT = 2, // TODO:
	EC_VERSION = 5, // Provide client version.
	ES_VERSION = 6, // TODO:

	// For login
	EC_LOGIN = 10, // with LoginData.
	ES_LOGIN = 11, // with LoginResult.
	EC_LOGOUT = 12, // LogoutData.
	ES_LOGOUT = 13, // refer LogoutResult
	EC_GET_LS_INFO = 14,
	ES_GET_LS_INFO = 15,
	EC_ADMIN_LOGIN = 10, // with LoginData.
	ES_ADMIN_LOGIN = 11, // with LoingResult.

	// For server list not only for login.
	EC_SERVER_LIST = 20, // with ServerListData
	ES_SERVER_LIST = 21, // with ServerListResult.
	EC_ENTER_SERVER = 22, // with enter server with server id only.
	ES_ENTER_SERVER = 23, // with EnterResult.

	EC_CRYPTO_KEY = 40, // for key..
	ES_CRYPTO_KEY = 41, // 
};
}
}

#endif
