#include "char.h"
#include "strlib.h"
#include "defs.h"
#include <cassert>
#include <cmath>

using namespace std;
using namespace srdgame;
namespace ro
{

size_t char_to_buf(char* buf, const RoCharInfo* info)
{
	if (buf == NULL || info == NULL)
		return 0;

	//printf("CHAR_TO_BUF, with name : %s\n", info->_name.c_str());
	size_t len = 0;
	PUINT32(buf, len) = info->_id;				len += 4;
	PUINT32(buf, len) = info->_exp._zeny;		len += 4;
	PUINT32(buf, len) = info->_exp._base_exp; 	len += 4;
	PUINT32(buf, len) = info->_exp._job_exp;	len += 4;
	PUINT32(buf, len) = info->_exp._job_lvl;	len += 4;
	PUINT32(buf, len) = 0;						len += 4;
	PUINT32(buf, len) = 0;						len += 4;
	PUINT32(buf, len) = info->_option;				len += 4;
	PUINT32(buf, len) = info->_prop._karma;		len += 4;
	PUINT32(buf, len) = info->_prop._manner;		len += 4;
	PUINT16(buf, len) = info->_exp._prop_point; 	len += 2;
	PUINT16(buf, len) = info->_prop._cur_hp;		len += 2;
	PUINT16(buf, len) = info->_prop._max_hp;		len += 2;
	PUINT16(buf, len) = info->_prop._cur_sp;		len += 2;
	PUINT16(buf, len) = info->_prop._max_sp;		len += 2;
	PUINT16(buf, len) = DEFAULT_WALK_SPEED;		len += 2;
	PUINT16(buf, len) = info->_class;				len += 2;
	PUINT16(buf, len) = info->_show._hair_style;			len += 2;
	PUINT16(buf, len) = info->_option & 0x20 ? 0 : info->_show._weapon;	len += 2; // when the weapon is sent and your option is riding, the client crashes on login!?
	PUINT16(buf, len) = info->_exp._base_lvl;		len += 2;
	PUINT16(buf, len) = info->_exp._skill_point;	len += 2;
	PUINT16(buf, len) = info->_show._head_bottom;	len += 2;
	PUINT16(buf, len) = info->_show._shield;		len += 2;
	PUINT16(buf, len) = info->_show._head_top;		len += 2;
	PUINT16(buf, len) = info->_show._head_middle;	len += 2;
	PUINT16(buf, len) = info->_show._hair_color;	len += 2;
	PUINT16(buf, len) = info->_show._clothes_color;	len += 2;
	memset(PCHAR(buf, len), 0, 24);
	memcpy(PCHAR(buf, len), info->_name.c_str(), min((int)info->_name.length(), 24)); len += 24;
	PUINT8(buf, len) = min(info->_prop._str, 255);		len += 1;
	PUINT8(buf, len) = min(info->_prop._agi, 255);		len += 1;
	PUINT8(buf, len) = min(info->_prop._vit, 255);		len += 1;
	PUINT8(buf, len) = min(info->_prop._int, 255);		len += 1;
	PUINT8(buf, len) = min(info->_prop._dex, 255);		len += 1;
	PUINT8(buf, len) = min(info->_prop._luk, 255);		len += 1;
	PUINT16(buf, len) = info->_slot;					len += 2;
	PUINT16(buf, len) = 1; 							len += 2;// Rename bit, 0 == rename, 1 == norename.
	assert(len == 108);
	return len;

}

size_t chars_to_buf(char* buf, const RoCharInfo* infos, size_t num)
{
	size_t len = 0;
	PUINT16(buf, len) = 0x6b; 		len += 4;// will wirte another 2 bytes later.
	memset(PCHAR(buf, len), 0, 20); len += 20;// unknown.
	for (size_t i = 0; i < num; ++i)
	{
		len += char_to_buf(buf + len, infos + i);
	}
	assert(len == 24 + num * 108);
	PUINT16(buf, 2) = len;
	return len;
	//return 24 + 9 * 108;
}

size_t delete_char_ok(char* buf, const RoCharInfo* info)
{
	size_t len = 0;
	PUINT16(buf, len) = 0x2b12;						len += 2;
	PUINT32(buf, len) = info->_id;					len += 4;
	PUINT32(buf, len) = info->_family._partner_id;	len += 4;
	return len;
}

size_t char_auth_refuse(char* buf)
{
	PUINT16(buf, 0) = 0x6c;
	PUINT8(buf, 2) = 0;
	return 3;
}

size_t char_auth_failed(char* buf, uint8 reason)
{
	PUINT16(buf, 0) = 0x81;
	PUINT8(buf, 2) = reason;
	return 3;
}
size_t from_login_to_char(Packet* dest, const char* src, size_t size)
{
	size_t res = 0;
	dest->op = EC_LOGIN_TO_CHAR;
	dest->len = sizeof(Packet) + sizeof(LoginToChar);
	LoginToChar* l = new LoginToChar();
	res = from_login_to_char(l, src, size);
	dest->param.Data = (char*)l;
	return res;
}

size_t from_login_to_char(LoginToChar* dest, const char* src, size_t src_len)
{
	if(src_len < 17)
		return 0;
	size_t len = 0;
	assert(PUINT16(src, len) == 0x65);		len += 2;

	dest->_account = PUINT32(src, len);		len += 4;
	dest->_id1 = PUINT32(src, len);			len += 4;
	dest->_id2 = PUINT32(src, len);			len += 4;
	len += 2; // TODO: what is these two bytes?
	dest->_sex = PUINT8(src, len);			len += 1;
	assert(len == 17);
	return len;
}
size_t to_login_to_char(char* src, uint32 account)
{
	PUINT32(src, 0) = account;
	return 4;
}
size_t from_select_char(Packet* dest, const char* src, size_t size)
{
	size_t res = 0;
	dest->op = EC_SELECT_CHAR;
	dest->len = sizeof(Packet);
	uint32 slot = 0;
	res = from_select_char(slot, src, size);
	dest->param.Int = (int)slot;
	return res;
}
size_t from_select_char(uint32& slot, const char* src, size_t src_len)
{
	if (src_len < 3)
		return 0;
	assert(PUINT16(src, 0) == 0x66);
	slot = PUINT8(src, 2);
	return 3;
}

size_t to_send_email_auth_failed(char* dest, uint8 reason)
{
	PUINT16(dest, 0) = 0x70;
	PUINT8(dest, 2) = reason; // 00 == Incorrect Email address.
	return 3;
}

size_t to_select_char_ok(char* dest, const MapServerInfo& info)
{
	size_t len = 0;
	PUINT16(dest, len) = 0x71;					len += 2;
	PUINT32(dest, len) = info._account; 		len += 4;
	memcpy(PCHAR(dest, len), info._map_name, 16); 	len += 16;
	PUINT32(dest, len) = htonl(info._ip);		len += 4;
	PUINT16(dest, len) = ntows(htons(info._port));	len += 2;
	assert(len == 28);
	return len;
}
size_t from_create_char(Packet* dest, const char* src, size_t size)
{
	size_t res = 0;
	dest->op = EC_CHAR_CREATE;
	dest->len = sizeof(Packet) + sizeof(CreateCharData);
	CreateCharData* data = new CreateCharData();
	res = from_create_char(*data, src, size);
	dest->param.Data = (char*)data;
	return res;
}
size_t from_create_char(CreateCharData& data, const char* buf, size_t src_len)
{
	if (src_len < 37)
		return 0;
	assert(PUINT16(buf, 0) == 0x67);
	size_t len = 2;
	/*assert(sizeof(CreateCharData) == 35);
	::memcpy(data._name, src, 35); // Just copy it :-).
	*/
	memcpy(data._name, PCHAR(buf, len), 24);	len += 24;
	data._str = PUINT8(buf, len);				len += 1;
	data._agi = PUINT8(buf, len);				len += 1;
	data._vit = PUINT8(buf, len);				len += 1;
	data._int = PUINT8(buf, len);				len += 1;
	data._dex = PUINT8(buf, len);				len += 1;
	data._luk = PUINT8(buf, len);				len += 1;
	data._slot = PUINT8(buf, len); 				len += 1;// The slot index for select characters.... -_-!
	data._hair_color = PUINT16(buf, len);		len += 2;
	data._hair_style = PUINT16(buf, len);		len += 2;

	assert(len == 37);

	return 37;
}
size_t to_create_char_failed(char* dest, uint8 reason)
{
	PUINT16(dest, 0) = 0x6e;
	PUINT8(dest, 2) = reason;
	return 3;
}
size_t to_create_char_ok(char* dest, RoCharInfo* info)
{
	PUINT16(dest, 0) = 0x6d;
	size_t len = 2;
	len += char_to_buf(dest + len, info);
	return len;
}

size_t from_delete_char(Packet* dest, const char* src, size_t size)
{
	size_t res = 0;
	dest->op = EC_CHAR_DELETE;
	dest->len = sizeof(Packet) + sizeof(DeleteCharData);
	DeleteCharData* data = new DeleteCharData();
	res = from_delete_char(*data, src, size);
	dest->param.Data = (char*)data;
	return res;
}

size_t from_delete_char(DeleteCharData& data, const char* src, size_t src_len)
{
	if (src_len < 2)
		return 0;
	size_t len = 0;
	uint16 op = PUINT16(src, len);			len += 2;
	if (op == 0x68 && src_len < 46)
		return 0;
	if (op == 0x1fb && src_len < 56)
		return 0;

	data._id = PUINT32(src, len);			len += 4;
	memcpy(data._email, PCHAR(src, len), 40); 	len += 40;

	if (op == 0x68)
		return 46;
	if (op == 0x1fb)
		return 56;
}

size_t to_delete_char_failed(char* dest, uint8 reason)
{
	return to_send_email_auth_failed(dest, reason);
}
size_t to_delete_char_ok(char* dest)
{
	PUINT16(dest, 0) = 0x6f;
	return 2;
}
size_t from_keep_alive(Packet* dest, const char* src, size_t size)
{
	size_t res = 0;
	dest->op = EC_CHAR_KEEP_ALIVE;
	dest->len = sizeof(Packet);
	uint32 account = 0;
	res = from_keep_alive(account, src, size);
	dest->param.Int = (int) account;
	return res;
}
size_t from_keep_alive(uint32& account, const char* src, size_t src_len)
{
	if (src_len < 6)
		return 0;
	assert(PUINT16(src, 0) == 0x187);
	account = PUINT32(src, 2);
	return 6;
}
size_t to_keep_alive(char* dest, uint32 account)
{
	PUINT16(dest, 0) = 0x187;
	PUINT32(dest, 2) = account;
	return 6;
}

size_t from_char_rename(Packet* dest, const char* src, size_t size)
{
	size_t res = 0;
	dest->op = EC_CHAR_RENAME;
	dest->len = sizeof(Packet) + sizeof(CharRenameData);
	CharRenameData* data = new CharRenameData();
	res = from_char_rename(*data, src, size);
	return res;
}

size_t from_char_rename(CharRenameData& data, const char* src, size_t src_len)
{
	if (src_len < 34)
		return 0;
	assert(PUINT16(src, 0) == 0x28d);
	assert(sizeof(data) == 32);
	::memcpy((char*)&data, src, 32);
	return 32;
}


}
