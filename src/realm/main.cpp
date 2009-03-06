#include "log.h"
#include "realmserver.h"
#include "config.h"

using namespace srdgame;
int main()
{
	LogSuccess("RealmSrv", "Starting");
	RealmServer s(string("./realm.conf"));
	s.run();
}
