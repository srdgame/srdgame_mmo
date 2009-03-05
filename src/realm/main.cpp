#include "log.h"
#include "realmserver.h"
#include "config.h"

using namespace srdgame;
int main()
{
	LogSuccess("RealmSrv", "Starting");
	RealmServer s("./realm.conf");
	s.run();
}
