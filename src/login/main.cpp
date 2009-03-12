#include "log.h"
#include "loginserver.h"
#include "config.h"
#include <string>
//#include <mcheck.h>

using namespace srdgame;
int main(int argc, char** argv)
{
	//mtrace();
	std::string filename = "login.conf";
	if (argc == 2)
	{
		filename = std::string(argv[1]);
	}
	LogSuccess("LoginSrv", "Starting....................");
	LoginServer s(filename);
	s.run();
//	muntrace();
}
