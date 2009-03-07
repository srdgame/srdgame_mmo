#include "log.h"
#include "loginserver.h"
#include "config.h"
#include <string>

using namespace srdgame;
int main(int argc, char** argv)
{
	std::string filename = "login.conf";
	if (argc == 2)
	{
		filename = std::string(argv[1]);
	}
	LogSuccess("LoginSrv", "Starting....................");
	LoginServer s(filename);
	s.run();
}
