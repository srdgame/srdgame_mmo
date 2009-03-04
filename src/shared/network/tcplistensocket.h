#ifndef TCP_LISTEN_SOCKET_H_
#define TCP_LISTEN_SOCKET_H_

#include "listensocket.h"

namespace srdgame
{
class TcpListenSocket : public ListenSocket
{
public:
	TcpListenSocket(const char* l_addr, int port);
	virtual ~TcpListenSocket();
	virtual void close();
	virtual bool is_open();
public:
	virtual void on_accept() = 0;
	virtual SOCKET get_fd() = 0;

protected:
	SOCKET _fd;
	SOCKET _temp_socket;
	struct sockaddr_in _addr;
	struct sockaddr_in _temp_addr;
	bool _open;
	int _len;
};
}

#endif
