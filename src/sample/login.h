#ifndef LOGIN_H_
#define LOGIN_H_

#include "defs.h"
#include <iostream>
#include <cstring>

int lan_subnetcheck(uint32 ip);

size_t to_refuse_connect(char* buf);
size_t from_userinfo(Packet* dest, const char* src, size_t size, uint16 opcode);
size_t to_auth_result(char* buf, const Packet* packet);

size_t to_server_list_result(char* buf, const Packet* packet);
size_t to_crypto_key(char* buf, const Packet* packet);

size_t from_get_loginserver_info(Packet* dest, const char* src, size_t size);
size_t to_get_loginserver_info(char* buf, const Packet* packet);

size_t from_admin_login(Packet* dest, const char* src, size_t size);
size_t to_admin_login_result(char* buf, const Packet* packet);
#endif

