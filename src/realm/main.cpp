#include "log.h"
#include "realmserver.h"
#include "config.h"

using namespace srdgame;
int main(int argc, char** argv)
{
	std::string filename = "realm.conf";
	if (argc == 2)
	{
		filename = std::string(argv[1]);
	}
	LogSuccess("LoginSrv", "Starting....................");
	RealmServer s(filename);
	s.run();
}
