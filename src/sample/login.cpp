#include "login.h"
#include "strlib.h"

size_t to_refuse_connect(char* buf)
{
	memset(buf, 0, 23);
	PINT16(buf, 0) = 0x6a;
	PINT8(buf, 2) = 3; // 3 = Rejected from server.
	return 23;
}
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
		res = 60
	}
	char* buf = new char[128 + 128];
	dest->op = EC_LOGIN;
	dest->len = sizeof(Packet) + 128 + 128;
	dest->param.Data = buf;

	memset(buf, 0, 128 + 128);

	uint32 version;
	char* name = buf;
	char* pass = buf + 128;
	unsigned char hash[16];
	uint8 client_type = 0;

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
		client_type = PUNIT8(src, 46);
	}
	if (!is_raw_pass)
	{
		bin2hex(pass, hash, 16);
	}
	return res;
}

size_t to_auth_result(char* buf, Packet* packet)
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
		// Do not need to send data?
		return 0;
	}

}

size_t to_server_list_result(char* buf, Packet* packet)
{
	long count = (*(long*)packet->param.Data);
	size_t res = 47 + 32 * count;
	memset(buf, 0, res);

	PUINT16(buf, 0) = 0x69; // opcode.
	PUINT16(buf, 2) = res; // length :-)

	char* other = packet->param.Data + sizeof(long) + count * sizeof(ServerInfo);
	uint32 id1 = PUINT32(other, 4);
	uint32 id2 = PUINT32(other, 8);
	uint32 account = PUINT32(other, 0);
	uint8 sex = PUINT8(other, 12);
	uint32 ip = PUINT32(other, 16);

	PUINT32(buf, 4) = id1;
	PUINT32(buf, 8) = account;
	PUINT32(buf, 12) = id2;
	PUINT32(buf, 16) = 0;

	PUINT16(buf, 44) = 0; // unknown
	PUINT8(buf, 46) = sex_str2num(sex);

	for (i = 0, n = 0; i < count; ++i)
	{
		ServerInfo* info = (ServerInfo*)(packet->param.Data + sizeof(long) + sizeof(ServerInfo) * i);
		uint32 subnet_char_ip = lan_subnetcheck(ip);
		PUINT32(buf, 47 + n * 32) = htonl((subnet_char_ip) ? subnet_char_ip : info->_ip);
		PUINT16(buf, 47 + n * 32 + 4) = ntows(htons((uint16)info->_port));

		memcpy(PCHAR(buf, 47 + n * 32 + 6), info->_name, 20);
		PUINT16(buf, 47 + n * 32 + 26) = PUINT16(info->_other, 0); // users
		PUINT16(buf, 47 + n * 32 + 28) = PUINT16(info->_other, 2); // maintenance
		PUINT16(buf, 47 + n * 32 + 28) = PUINT16(info->_other, 4); // new_
		n++;
	}
	return res;
}

size_t to_crypto_key(char* buf, Packet* packet)
{
	size_t md5keylen = 12; // take the less one.
	size_t res = 4 + 12;
	
	PUINT16(buf, 0) = 0x01dc; // opcode.
	PUINT16(buf, 2) = res;
	memcpy(PCHAR(buf, 4), packet->_key, md5keylen);

	return res;
}

size_t from_get_loginserver_info(Packet* dest, const char* src, size_t size)
{
	size_t res = 2;
	dest->op = EC_GET_LS_INFO;
	dest->len = sizeof(Packet);
	dest->param.Lang = 0;

}

size_t to_get_loginserver_info(char* buf, Packet* packet)
{
	size_t res = 10;
	PUINT16(buf, 0) = 0x7531;
	memcpy(PCHAR(buf, 2), "01000000", 8);
	return res;
}

size_t from_admin_login(Packet* dest, const char* src,  size_t size)
{
	if (size < 2)
		return 0;

	int passwd_enc = (int)PUINT16(src, 2);
	const char* pass = PCHAR(src, 4);

	if (passwd_enc == 0)
	{
		if (size < 28)
			return 0;
		char* buf = new char[24];
		dest.param.Data = buf;
		memcpy(buf, pass, 24);
	}
	else
	{
		if (size < 20)
			return 0;
		char* buf = new char[16];
		memcpy(buf, pass, 16);
	}
	size_t res = (passwd_enc == 0) ? 28 : 20;
	dest->op = EC_ADMIN_LOGIN;
	dest->len = sizeof(Packet) + res - 4;
	reeturn res;
}

size_t to_admin_login_result(char* buf, Packet* packet)
{
	size_t res = 3;
	PUINT16(buf, 0) = 0x7919;
	PUINT8(buf, 2) = PUINT64(packet->param.Data) == 0 ? 0, : 1;
	return res;
}
