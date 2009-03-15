#include "defs.h"
#include <iostream>
#include <cstring>
#include "login.h"

using namespace srdgame;
using namespace srdgame::opcode;

size_t from_stream(Packet* dest, const char* src, size_t size)
{
	dest->op = EC_NONE;
	size_t res = 0;
	if (size <= 2)
	{
		return res;
	}
	uint16 opcode = PUINT16(src, 0);
	switch (opcode)
	{
		case 0x0200: // Alive packket`
			if (size >= 26)
			{
				res = 26; // Just remove them from the bufer.
			}
			break;
		case 0x0204: // Client md5 hash?
			if (size >= 18)
			{
				res = 18;
			}
			break;
		case 0x0064:
		case 0x0277:
		case 0x02b0:
		case 0x01dd:
		case 0x01fa:
		case 0x027c: //login with username password.
			res = from_userinfo(dest, src, size, opcode);
			break;
		case 0x01db: // get md5 key.
		case 0x791a: // for md5 key.
			if (size >= 2)
			{
				res = 2;
				dest->op = EC_CRYPTO_KEY;
			}
			break;
		case 0x7530: // get loginserver's info
			if (size >= 2)
			{
				res = from_get_loginserver_info(dest, src, size);
			}
		case 0x7918:
			// Admin login?
			res = from_admin_login(dest, src, size);
			break;
		default:
			break;

	}
	std::cout << "In from_stream function!!!!" << std::endl;
	return res;
}
size_t to_stream(char* dest, const Packet* src)
{
	size_t res = 0;
	switch (src->op)
	{
		case ES_INIT:
			if (!src->param.Bool)
			{
				res = to_refuse_connect(dest);
			}
			break;
		case ES_LOGIN:
			res = to_auth_result(dest, src);
			break;
		case ES_CRYPTO_KEY:
			res = to_crypto_key(dest, src);
			break;
		case ES_GET_LS_INFO:
			res = to_get_loginserver_info(dest, src);
			break;
		case ES_ADMIN_LOGIN:
			res = to_admin_login_result(dest, src);
			break;
		case ES_SERVER_LIST:
			res = to_server_list_result(dest, src);
			break;
		default:
			break;
	}
	std::cout << "In to_stream function!!!!" << std::endl;
	return res;
}


