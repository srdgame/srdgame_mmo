#include "log.h"
#include "realmserver.h"
#include "config.h"
#include <string>
//#include <mcheck.h>

using namespace srdgame;
int main(int argc, char** argv)
{
	//mtrace();
	std::string filename = "realm.conf";
	if (argc == 2)
	{
		filename = std::string(argv[1]);
	}
	LogSuccess("RealmServer", "Starting....................");
	RealmServer s(filename);
	s.run();
//	muntrace();
}
