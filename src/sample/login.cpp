#include "login.h"
#include "strlib.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include "ro_defs.h"
#include "share.h"

namespace ro
{

size_t to_refuse_connect(char* buf)
{/*
	memset(buf, 0, 23);
	PINT16(buf, 0) = 0x6a;
	PINT8(buf, 2) = 3; // 3 = Rejected from server.
	return 23;*/
	send_6a_to_disconnect(buf, 3);
}
#define CONVIP(ip) ((ip)>>24)&0xFF,((ip)>>16)&0xFF,((ip)>>8)&0xFF,((ip)>>0)&0xFF

size_t from_userinfo(Packet* dest, const char* src, size_t size, uint16 opcode)
{
	size_t res = 0;

	if ((opcode  == 0x0064 && size < 55)){
		return 0;
	}else{
		res = 55;
	}
	if (opcode == 0x0277 && size < 84){
		return 0;
	}else{
		res = 84;
	}
	if (opcode == 0x02b0 && size < 85){
		return 0;
	}else{
		res = 85;
	}
	if (opcode == 0x01dd && size < 47){
		return 0;
	}else{
		res = 47;
	}
	if (opcode == 0x01fa && size < 48) {
		return 0;
	}else{
		res = 48;
	}
	if (opcode == 0x027c && size < 60){
		return 0;
	}else{
		res = 60;
	}
	char* buf = new char[sizeof(LoginInfo)];// enough.
	memset(buf, 0, sizeof(LoginInfo));

	dest->op = EC_LOGIN;
	dest->len = sizeof(Packet) + sizeof(LoginInfo);
	dest->param.Data = buf;


	LoginInfo info;

	uint32& version = info.version;
	char* name = info.name;
	memset(name, 0, 24);
	char *pass = info.pass;
	memset(pass, 0, 32);
	unsigned char hash[16];
	uint8& client_type = info.client_type;

	bool is_raw_pass = (opcode == 0x0064 || opcode == 0x0277 || opcode == 0x02b0);

	version = PUINT32(src, 2);
	memcpy(name, PCHAR(src, 6), 24);
	if (is_raw_pass)
	{
		memcpy(pass, PCHAR(src, 30), 24);
		client_type = PUINT8(src, 54);
	}
	else
	{
		memcpy(hash, PCHAR(src, 30), 16);
		client_type = PUINT8(src, 46);
	}
	if (!is_raw_pass)
	{
		bin2hex(pass, hash, 16); // pass to 32 bytes?
	}

	memcpy(buf, (char*)(&info), sizeof(LoginInfo));	
/*
	size_t len = 0;
	// Save name
	PUINT16(buf, len) = (uint16)LP_NAME;		len += 2;
	PUINT16(buf, len) = 24;						len += 2;
	memcpy(PCHAR(buf, len), name, 24);			len +=24;

	// Save password.
	PUINT16(buf, len) = (uint16)LP_PASS;		len += 2;
	PUINT16(buf, len) = 32;						len += 2;
	memcpy(PCHAR(buf, len), pass, 32);			len += 32;

	// Save crypto type
	PUINT16(buf, len) = (uint16)LP_CRYPTO_TYPE; len += 2;
	PUINT16(buf, len) = 1;						len += 2;
	PUINT8(buf, len) = is_raw_pass ? 0x00 : 0x03; // Try both auth with 0x03
	len += 1;

	// save client type.
	PUINT16(buf, len) = (uint16)ROLP_CLIENT_TYPE;	len += 2;
	PUINT16(buf, len) = 1;							len += 2;
	PUINT8(buf, len) = client_type;					len += 1;

	// save version.
	PUINT16(buf, len) = (uint16)ROLP_VERSION;	len += 2;
	PUINT16(buf, len) = 4;						len += 2;
	PUINT32(buf, len) = version;				len += 4;

	dest->len += len;

	*/

	return res;
}

size_t to_auth_result(char* buf, const Packet* packet)
{
	size_t res = 0;
	if (packet->param.Long)
	{
		// Login errors.
		res = 3;
		PINT16(buf, 0) = 0x81;
		switch (packet->param.Long)
		{
			case 1:
				PINT8(buf, 2) = 1; // Server closed?
				break;
			case 2:
				PINT8(buf, 2) = 8; // char still in here.
				break;
			default:
				PINT8(buf, 2) = 1;
				break;
		}
		return res;
	}
	else
	{
		// Do not need to send data?  Yes, we need to send the server list.
		return 0;
	}

}

size_t to_server_list_result(char* buf, const Packet* packet)
{
	ServerListHeader* header = (ServerListHeader*)packet->param.Data;
	uint16 count = header->server_count;
	size_t res = 47 + 32 * count;
	memset(buf, 0, res);

	PUINT16(buf, 0) = 0x69; // opcode.
	PUINT16(buf, 2) = res; // length :-)

	uint32 id1 = header->id1;
	uint32 id2 = header->id2;
	uint32 account = header->account;
	uint8 sex = header->sex;
	uint32 ip = header->ip;

	PUINT32(buf, 4) = id1;
	PUINT32(buf, 8) = account;
	PUINT32(buf, 12) = id2;
	PUINT32(buf, 16) = 0;

	PUINT16(buf, 44) = 0; // unknown
	PUINT8(buf, 46) = sex_str2num(sex);

	int i = 0;
	for (; i < count; ++i)
	{
		ServerInfo* info = (ServerInfo*)(packet->param.Data + sizeof(ServerListHeader) + sizeof(ServerInfo) * i);
		uint32 subnet_char_ip = lan_subnetcheck(ip);
		PUINT32(buf, 47 + i * 32) = htonl((subnet_char_ip) ? subnet_char_ip : info->ip);
		printf("IP of Server : %d.%d.%d.%d.\n", CONVIP(info->ip));
		printf("Port of Server : %d\n", info->port);

		PUINT16(buf, 47 + i * 32 + 4) = ntows(htons(info->port));

		memcpy(PCHAR(buf, 47 + i * 32 + 6), info->name, 20);
		PUINT16(buf, 47 + i * 32 + 26) = info->users;// users
		PUINT16(buf, 47 + i * 32 + 28) = info->maintenance; // maintenance
		PUINT16(buf, 47 + i * 32 + 30) = info->new_; // new_
	}
	return res;
}

size_t to_crypto_key(char* buf, const Packet* packet)
{
	RO_MD5Key* key = (RO_MD5Key*)(packet->param.Data);
	size_t md5keylen = 16; // take the less one.
	size_t res = 4 + 12;
	
	PUINT16(buf, 0) = 0x01dc; // opcode.
	PUINT16(buf, 2) = res;
	memcpy(PCHAR(buf, 4), key->key, md5keylen);

	return res;
}

size_t from_get_loginserver_info(Packet* dest, const char* src, size_t size)
{
	size_t res = 2;
	dest->op = EC_GET_LS_INFO;
	dest->len = sizeof(Packet);
	dest->param.Long = 0;

}

size_t to_get_loginserver_info(char* buf, const Packet* packet)
{
	LoginServerInfo * info = (LoginServerInfo*)packet->param.Data;
	size_t res = 10;
	PUINT16(buf, 0) = 0x7531;
	memcpy(PCHAR(buf, 2), info->info, 8);
	return res;
}

size_t from_admin_login(Packet* dest, const char* src,  size_t size)
{
	if (size < 2)
		return 0;

	AdminLoginInfo info;
	memset(info.pass, 0, 32);

	info.crypto_type  = (int)PUINT16(src, 2);
	const char* pass = PCHAR(src, 4);

	if (info.crypto_type == 0)
	{
		if (size < 28)
			return 0;
		memcpy(info.pass, pass, 24);
	}
	else
	{
		if (size < 20)
			return 0;
		memcpy(info.pass, pass, 16);
	}
	size_t res = (info.crypto_type == 0) ? 28 : 20;
	dest->op = EC_ADMIN_LOGIN;
	dest->len = sizeof(Packet) + sizeof(AdminLoginInfo);
	char * buf = new char[sizeof(AdminLoginInfo)];
	memcpy(buf, (char*)&info, sizeof(AdminLoginInfo));
	return res;
}

size_t to_admin_login_result(char* buf, const Packet* packet)
{
	size_t res = 3;
	PUINT16(buf, 0) = 0x7919;
	PUINT8(buf, 2) = PUINT64(packet->param.Data, 0) == 0 ? 0 : 1;
	return res;
}

}
