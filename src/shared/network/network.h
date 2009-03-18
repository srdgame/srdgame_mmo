#ifndef NETWORK_H_
#define NETWORK_H_

#include "socketmgr.h"
#include "socket.h"
#include "tcpsocket.h"
#include "tcplistensocket.h"

namespace srdgame
{
inline std::string get_host_ip()
{
	char host_name[512]; // more than enough?
	::memset(host_name, 0, 512);

	if (0 != gethostname(host_name, 512))
	{
		return std::string("");
	}
	hostent* host = gethostbyname(host_name);
	if(!host)
	{
		return std::string("");
	}
	return std::string(host->h_addr_list[0]);
}
}
#endif
