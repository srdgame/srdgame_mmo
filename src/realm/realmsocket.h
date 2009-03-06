#ifndef REALM_SOCKET_H_
#define REALM_SOCKET_H_

#include "network.h"

namespace srdgame
{
class RealmSocket : public TcpSocket
{
public:
	virtual void on_rev();
	virtual void on_send();
	virtual void on_connect();
	virtual void on_close();
};
}

#endif
