#include "defs.h"
#include <iostream>
#include <cstring>
#include "login.h"
#include "char.h"
#include "ro_defs.h"
#include "map_main.h"

using namespace srdgame;
using namespace srdgame::opcode;
using namespace ro;

void init()
{
	std::cerr << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;
	// Actually load the configuration db files,
	FromDispatch::get_singleton().init();
	// Just map the functions with opcode.
	ToDispatch::get_singleton().init();
}

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
		// Login Server used.
		//
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
			res = from_userinfo(dest, src, size);
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
		// Char server used.
		//
		case 0x0065:
			{
				dest->op = EC_LOGIN_TO_CHAR;
				dest->len = sizeof(Packet) + sizeof(LoginToChar);
				LoginToChar* l = new LoginToChar();
				res = from_login_to_char(l, src, size);
				dest->param.Data = (char*)l;
			}
			break;
		case 0x0066:
			{
				dest->op = EC_SELECT_CHAR;
				dest->len = sizeof(Packet);
				uint32 slot = 0;
				res = from_select_char(slot, src, size);
				dest->param.Int = (int)slot;
			}
			break;
		case 0x0067:
			{
				dest->op = EC_CHAR_CREATE;
				dest->len = sizeof(Packet) + sizeof(CreateCharData);
				CreateCharData* data = new CreateCharData();
				res = from_create_char(*data, src, size);
				dest->param.Data = (char*)data;
			}
			break;
		case 0x0068:
		case 0x01fb:
			{
				dest->op = EC_CHAR_DELETE;
				dest->len = sizeof(Packet) + sizeof(DeleteCharData);
				DeleteCharData* data = new DeleteCharData();
				res = from_delete_char(*data, src, size);
				dest->param.Data = (char*)data;
			}
			break;
		case 0x0187:
			{
				dest->op = EC_CHAR_KEEP_ALIVE;
				dest->len = sizeof(Packet);
				uint32 account = 0;
				res = from_keep_alive(account, src, size);
				dest->param.Int = (int) account;
			}
			break;
		case 0x028d:
			{
				dest->op = EC_CHAR_RENAME;
				dest->len = sizeof(Packet) + sizeof(CharRenameData);
				CharRenameData* data = new CharRenameData();
				res = from_char_rename(*data, src, size);
			}
			break;
		default:
			res = map_from_stream(dest, src, size);
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

		// For Map server
		case ES_LOGIN_TO_CHAR:
			{
				if (src->param.Int != 0)
				{
					printf("ES_LOGIN_TO_CHAR, with account id : %d", src->param.Int);
					res = to_login_to_char(dest, src->param.Int);
				}
				else
				{
					res = char_auth_refuse(dest);
				}
			}break;
		case ES_CHAR_LIST:
			{
				CharDataList* list =((CharDataList*) src->param.Data);
				printf("ES_CHAR_LIST, with character count : %d", list->_count);
				res = chars_to_buf(dest, list->_chars, list->_count);
			}break;
		case ES_SELECT_CHAR:
			{
				if (src->len == sizeof(Packet))
				{
					uint8 reason = src->param.Char;
					res = to_send_email_auth_failed(dest, reason);
				}
				else
				{
					MapServerInfo* info = (MapServerInfo*)src->param.Data;
					res = to_select_char_ok(dest, *info);
				}
			}
			break;
		case ES_CHAR_CREATE:
			{
				if (src->len == sizeof(Packet))
				{
					uint8 reason = src->param.Char;
					res = to_create_char_failed(dest, reason);
				}
				else
				{
					res = to_create_char_ok(dest, (RoCharInfo*)src->param.Data);
				}
			}
			break;
		case ES_CHAR_DELETE:
			{
				if (src->param.Char == 0)
				{
					res = to_delete_char_ok(dest);
				}
				else
				{
					res = to_delete_char_failed(dest, src->param.Char);
				}
			}
			break;
		case ES_CHAR_RENAME:
			res = 0;
			break;
		case ES_CHAR_KEEP_ALIVE:
			res = to_keep_alive(dest, src->param.Int);
			break;
		default:
			res = map_to_stream(dest, src);
			break;
	}
	std::cout << "In to_stream function!!!!" << std::endl;
	return res;
}


