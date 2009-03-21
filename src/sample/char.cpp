#include "char.h"

size_t char_to_buf(char* buf, const RoCharInfo* info)
{
	if (buf == NULL || info == NULL)
		return 0;

	size_t len = 0;
	PUINT32(buf, len) = info->_id;				len += 4;
	PUINT32(buf, len) = info->_exp._base_exp; 	len += 4;
	PUINT32(buf, len) = info->_exp._job_exp;	len += 4;
	PUINT32(buf, len) = info->_exp._job_level;	len += 4;
	PUINT32(buf, len) = 0;						len += 4;
	PUINT32(buf, len) = 0;						len += 4;
	PUINT32(buf, len) = p->_option;				len += 4;
	PUINT32(buf, len) = p->_prop._karma;		len += 4;
	PUINT32(buf, len) = p->_prop._manner;		len += 4;
	PUINT16(buf, len) = p->_exp._prop_point; 	len += 2;
	PUINT16(buf, len) = p->_prop._cur_hp;		len += 2;
	PUINT16(buf, len) = p->_prop._max_hp;		len += 2;
	PUINT16(buf, len) = p->_prop._cur_sp;		len += 2;
	PUINT16(buf, len) = p->_prop._max_sp;		len += 2;
	PUINT16(buf, len) = DEFAULT_WALK_SPEED;		len += 2;
	PUINT16(buf, len) = p->_class;				len += 2;
	PUINT16(buf, len) = p->_show._hair;			len += 2;
	PUINT16(buf, len) = p->_option & 0x20 ? 0 : p->weapon;	len += 2; // when the weapon is sent and your option is riding, the client crashes on login!?
	PUINT16(buf, len) = p->_exp._base_lvl;		len += 2;
	PUINT16(buf, len) = p->_exp._skill_point;	len += 2;
	PUINT16(buf, len) = p->_show._head_bottom;	len += 2;
	PUINT16(buf, len) = p->_show._shield;		len += 2;
	PUINT16(buf, len) = p->_show._head_top;		len += 2;
	PUINT16(buf, len) = p->_show._head_middle;	len += 2;
	PUINT16(buf, len) = p->_show._hair_color;	len += 2;
	PUINT16(buf, len) = p->_show.clothes_color;	len += 2;
	memset(PCHAR(buf, len), 0, 24);
	memcpy(PCHAR(buf, len), p->_name.c_sr(), p->_name.length()); len += 24;
	PUINT8(buf, len) = min(p->_prop._str, 255);		len += 1;
	PUINT8(buf, len) = min(p->_prop._agi, 255);		len += 1;
	PUINT8(buf, len) = min(p->_prop._vit, 255);		len += 1;
	PUINT8(buf, len) = min(p->_prop._int, 255);		len += 1;
	PUINT8(buf, len) = min(p->_prop._dex, 255);		len += 1;
	PUINT8(buf, len) = min(p->_prop._luk, 255);		len += 1;
	PUINT16(buf, len) = min(p->_show._slot);		len += 2;
	PUINT16(buf, len) = 1; 							len += 2;// Rename bit, 0 == rename, 1 == norename.
	assert(len == 108);
	return len;

}

size_t chars_to_buf(char* buf, const RoCharInfo* infos, size_t num)
{
	size_t len = 0;
	PUINT16(buf, len) = 0x6b; 		len += 4;// will wirte another 2 bytes later.
	memset(PCHAR(buf, len), 0, 20); // unknown.
	for (size_t i = 0; i < num; ++i)
	{
		len += char_to_buf(buf + len, infos + i);
	}
	assert(len == 24 + num * 108);
	PUINT16(buf, 2) = len;
	return len;
}

size_t delete_char_ok(char* buf, const RoCharInfo* info)
{
	size_t len = 0;
	PUINT16(buf, len) = 0x2b12;						len += 2;
	PUINT32(buf, len) = info->_id;					len += 4;
	PUINT32(buf, len) = info->_family._parter_id;	len += 4;
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

size_t from_login_to_char(LoginToChar* dest, char* src, size_t src_len)
{
	if(src_len < 17)
		return 0;
	size_t len = 0;
	assert(PUINT16(src, len) == 0x65);		len += 2;

	dest._account = PUINT32(src, len);		len += 4;
	dest._id1 = PUINT32(src, len);			len += 4;
	dest._id2 = PUINT32(src, len);			len += 4;
	len += 2; // TODO: what is these two bytes?
	dest._sex = PUINT8(src, len);			len += 1;
	assert(len == 17);
	return len;
}
size_t to_login_to_char(char* src, uint32 account)
{
	PUINT32(src, 4) = account;
	return 4;
}
size_t from_select_char(uint32& slot, char* src, size_t src_len)
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
	memcpy(PCHAR(dest, len), info._name, 18); 	len += 16;
	PUINT32(dest, len) = htonl(info._ip);		len += 4;
	PUINT16(dest, len) = ntows(htons(info._port));	len += 2;
	assert(len == 28);
	return len;
}

size_t from_create_char(CreateCharData& data, char* src, size_t src_len)
{
	if (src_len < 37)
		return 0;
	assert(PUINT16(src, 0) == 0x67);
	size_t len = 0;
	assert(sizeof(ChreateCharData) == 35);
	::memcpy(data._name, src, 35); // Just copy it :-).
	return 37;
}
size_t to_create_char_failed(char* dest, uint8 reason)
{
	PUINT16(dest, 0) = 0x6e;
	PUINT8(dest, 2) = reason;
	return 3;
}

size_t from_delete_char(DeleteCharData& data, char* src, size_t src_len)
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
	return to_email_auth_failed(dest, reason);
}
size_t to_delete_char_ok(char* dest)
{
	PUINT16(dest, 0) = 0x6f;
	return 2;
}
size_t from_keep_aliv(uint32 account, char* src, size_t src_len)
{
	if (src_len < 6)
		return 0;
	assert(PUINT16(src, 0) == 0x187);
	account = PUINT32(src, 2);
	return 6;
}

size_t from_char_rename(CharRenameData& data, char* src, size_t src_len)
{
	if (src_len < 34)
		return 0;
	assert(PUINT16(src, 0) == 0x28d);
	assert(sizeof(data) == 32);
	::memcpy((char*)&data, src, 32);
	return 32;
}
