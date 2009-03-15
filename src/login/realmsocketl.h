#ifndef LOGIN_INTER_SOCKET_R_H_
#define LOGIN_INTER_SOCKET_R_H_

#include "loginsocketbase.h"
#include "packetdefs.h"


namespace srdgame
{
class LoginWorker;
struct Packet;
class LoginServer;

// Handle the communication with world server.
class LoginInterSocketR : public LoginSocketBase
{
	friend class LoginWorker;
	friend class LoginServer;
public:
	LoginInterSocketR(LoginServer* server);
	virtual ~LoginInterSocketR();
	//virtual void on_rev();
	virtual void on_send();
	virtual void on_connect();
	virtual void on_close();

public:
	// Worker will call this to response one packet.
	virtual void on_handle(Packet* packet);
protected:
	LoginServer* _server;
};


}

#endif
