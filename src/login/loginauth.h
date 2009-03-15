#ifndef LOGIN_AUTH_H_
#define LOGIN_AUTH_H_

#include "singleton.h"

namespace srdgame
{
class LoginSocket;
class Packet;
class LoginAuth : public Singleton < LoginAuth >
{
public:
	LoginAuth();
	~LoginAuth();

	void handle_login(LoginSocket* socket, const Packet* packet);
	void handle_admin_login(LoginSocket* socket, const Packet* packet);

	void get_crypto_key(LoginSocket* socket, const Packet* packet);

};
}

#endif
