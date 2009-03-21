#ifndef STR_LIB_H_
#define STR_LIB_H_

#include <cstddef>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <cstring>
#include "metadefs.h"

using namespace srdgame;

namespace ro
{

bool bin2hex(char* out, unsigned char* in, size_t size);

#define sex_num2str(num) ( (num ==  SEX_FEMALE  ) ? 'F' : (num ==  SEX_MALE  ) ? 'M' : 'S' )
#define sex_str2num(str) ( (str == 'F' ) ?  SEX_FEMALE  : (str == 'M' ) ?  SEX_MALE  :  SEX_SERVER  )

// Resolves hostname into a numeric ip.
inline uint32 host2ip(const char* hostname)
{
	struct hostent* h = gethostbyname(hostname);
	return (h != NULL) ? ntohl(*(uint32*)h->h_addr) : 0;
}

// Converts a numeric ip into a dot-formatted string.
// Result is placed either into a user-provided buffer or a static system buffer.
inline const char* ip2str(uint32 ip, char ip_str[16])
{
	struct in_addr addr;
	addr.s_addr = htonl(ip);
	return (ip_str == NULL) ? inet_ntoa(addr) : strncpy(ip_str, inet_ntoa(addr), 16);
}

// Converts a dot-formatted ip string into a numeric ip.
inline uint32 str2ip(const char* ip_str)
{
	return ntohl(inet_addr(ip_str));
}

//--------------------------------------------
// Test to know if an IP come from LAN or WAN.
//--------------------------------------------
inline int lan_subnetcheck(uint32 ip)
{
	/*
	int i;
	ARR_FIND( 0, subnet_count, i, (subnet[i].char_ip & subnet[i].mask) == (ip & subnet[i].mask) );
	return ( i < subnet_count ) ? subnet[i].char_ip : 0;*/
	return 0;
}
inline uint16 ntows(uint16 netshort)
{
	return ((netshort & 0xFF) << 8) | ((netshort & 0xFF00) >> 8);
}
}
#endif
