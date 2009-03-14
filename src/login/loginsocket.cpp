#include "loginsocket.h"
#include "log.h"
#include "packetparser.h"
#include "loginworker.h"

using namespace srdgame;

LoginSocket::LoginSocket()
	: LoginSocketBase(/* using the default buffer size */)
{
}

LoginSocket::~LoginSocket()
{
	if (_worker && _worker->is_running())
	{
		_worker->shutdown();
	}
}
void LoginSocket::on_send()
{
}

void LoginSocket::on_connect()
{
}

void LoginSocket::on_close()
{
}

void LoginSocket::on_handle(Packet* packet)
{
	switch (packet->op)
	{
		default:
			break;
	}
}

