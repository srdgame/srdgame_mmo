#include "security.h"

using namespace srdgame;

Security::Security()
{
}
Security::~Security()
{
}

bool Security::check_account(long id)
{
	return false;
}

bool Security::ip_check(sockaddr_in addr)
{
	return false;
}

