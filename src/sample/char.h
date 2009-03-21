#ifndef RO_CHAR_H_
#define RO_CHAR_H_

#include "metadefs.h"
#include "ro_defs.h"
#include "rocharinfo.h"

namespace ro
{

size_t char_to_buf(char* buf, const RoCharInfo* info);
size_t chars_to_buf(char* buf, const RoCharInfo* infos, size_t num);

size_t delete_char_ok(char* buf, const RoCharInfo* info);

size_t char_auth_refuse(char* buf);

size_t char_auth_failed(char* buf, uint8 reason);// auth ok is just send the player list, like login auth case.

// request to connect
		// 0065 <account id>.L <login id1>.L <login id2>.L <???>.W <sex>.B
size_t from_login_to_char(LoginToChar* dest, const char* src, size_t src_len);
size_t to_login_to_char(char* src, uint32 account);

// Char select
size_t from_select_char(uint32& slot, const char* src, size_t src_len);
size_t to_send_email_auth_failed(char* dest, uint8 reason);
size_t to_select_char_ok(char* dest, const MapServerInfo& info);

// create new char
// S 0067 <name>.24B <str>.B <agi>.B <vit>.B <int>.B <dex>.B <luk>.B <slot>.B <hair color>.W <hair style>.W
size_t from_create_char(CreateCharData& data, const char* src, size_t src_len);
size_t to_create_char_failed(char* dest, uint8 reason);
size_t to_create_char_ok(char* dest, RoCharInfo* info);
//

// delete char.
size_t from_delete_char(DeleteCharData& data, const char* src, size_t src_len);
size_t to_delete_char_failed(char* dest, uint8 reason);
size_t to_delete_char_ok(char* dest);

// Keep-alive packet.
size_t from_keep_alive(uint32& account, const char* src, size_t src_len);
size_t to_keep_alive(char* src, uint32 account);

// Rename packet
// R 028d <account ID>.l <char ID>.l <new name>.24B
size_t from_char_rename(CharRenameData& data, const char* src, size_t src_len);
}
#endif
