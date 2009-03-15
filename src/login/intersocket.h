#ifndef LOGIN_INTER_SOCKET_H_
#define LOGIN_INTER_SOCKET_H_

#include "loginsocketbase.h"
#include "packetdefs.h"


namespace srdgame
{
class LoginWorker;
struct Packet;

// Handle the communication with game clients.
class InterSocket : public LoginSocketBase
{
	friend class LoginWorker;
	friend class LoginAuth;
public:
	InterSocket();
	virtual ~InterSocket();
	//virtual void on_rev();
	virtual void on_send();
	virtual void on_connect();
	virtual void on_close();

public:
	// Worker will call this to response one packet.
	virtual void on_handle(Packet* packet);

protected:
};
}

#endif
