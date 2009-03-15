#ifndef STR_LIB_H_
#define STR_LIB_H_
#include <cstddef>

#include "metadefs.h"

using namespace srdgame;

bool bin2hex(char* out, unsigned char* in, size_t size);

#define sex_num2str(num) ( (num ==  SEX_FEMALE  ) ? 'F' : (num ==  SEX_MALE  ) ? 'M' : 'S' )
#define sex_str2num(str) ( (str == 'F' ) ?  SEX_FEMALE  : (str == 'M' ) ?  SEX_MALE  :  SEX_SERVER  )

// hostname/ip conversion functions
uint32 host2ip(const char* hostname);
const char* ip2str(uint32 ip, char ip_str[16]);
uint32 str2ip(const char* ip_str);

#endif
