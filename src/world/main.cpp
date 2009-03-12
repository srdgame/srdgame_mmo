#include "log.h"
#include "worldserver.h"
#include "config.h"
#include <string>
//#include <mcheck.h>

using namespace srdgame;
int main(int argc, char** argv)
{
	//mtrace();
	std::string filename = "world.conf";
	if (argc == 2)
	{
		filename = std::string(argv[1]);
	}
	LogSuccess("WorldSrv", "Starting....................");
	WorldServer s(filename);
	s.run();
//	muntrace();
}
