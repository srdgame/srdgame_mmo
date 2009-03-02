#ifndef NETWORK_CONFIG_H_
#define NETWORK_CONFIG_H_


#include "singleton.h"
#include "socket.h"

namespace srdgame
{
class NetworkConfig : public Singleton<NetworkConfig>
{
public:
	bool socket_check(Socket* s)
	{
		return true;
	}
};
}

#endif
