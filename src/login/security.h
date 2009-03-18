#ifndef SECURITY_H_
#define SECURITY_H_

#include "singleton.h"
#include "network.h"

namespace srdgame
{
class Security : public Singleton < Security >
{
public:
	Security();
	~Security();
public:
	bool check_account(long id);
	bool ip_check(sockaddr_in addr);
};
}

#endif
