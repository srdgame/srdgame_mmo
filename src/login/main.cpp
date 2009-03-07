#include "log.h"
#include "loginserver.h"
#include "config.h"

using namespace srdgame;
int main()
{
	LogSuccess("LoginSrv", "Starting");
	LoginServer s(string("./realm.conf"));
	s.run();
}
