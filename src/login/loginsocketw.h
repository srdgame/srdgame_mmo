#ifndef LOGIN_INTER_SOCKET_W_H_
#define LOGIN_INTER_SOCKET_W_H_

#include "loginsocketbase.h"
#include "packetdefs.h"
#include "fastqueue.h"


namespace srdgame
{
class LoginWorker;
struct Packet;

// Handle the communication with world server.
class LoginInterSocketW : public LoginSocketBase
{
	friend class LoginWorker;
public:
	LoginInterSocketW();
	virtual ~LoginInterSocketW();
	virtual void on_rev();
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
