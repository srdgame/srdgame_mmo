#ifndef TCP_LISTEN_SOCKET_H_
#define TCP_LISTEN_SOCKET_H_

#include "listensocket.h"
#include "tcpsocket.h"
#include "socketmgr.h"
#include "socketfun.h"
#include "log.h"

namespace srdgame
{
template <class T>
class TcpListenSocket : public ListenSocket
{
public:
	TcpListenSocket(const char* l_addr, int port)
		: ListenSocket()
		  , _fd(0)
		  , _temp_fd(0)
		  , _port(port)
		  , _open(false)
	{
		//LogDebug("Network", "Trying to listen on: %d", port);
		// Initialize the socket.
		_addr_size = sizeof(sockaddr_in);
		_fd = SocketFunc::create_tcp_socket();
		SocketFunc::reuse_addr(_fd);
		SocketFunc::nonblocking(_fd);
		SocketFunc::set_timeout(_fd, 60);
	
		// setup address
		_addr.sin_family = AF_INET;
		_addr.sin_port = ntohs((unsigned short)port);
		_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
		if (strcmp(l_addr, "0.0.0.0"))
		{
			// What is this?
			struct hostent* hostname = ::gethostbyname(l_addr);
			if (!hostname)
			{
				::memcpy(&_addr.sin_addr.s_addr, hostname->h_addr_list[0], hostname->h_length);
			}
		}

		// bind and listen.
		int re = ::bind(_fd, (const sockaddr*)&_addr, _addr_size);
		if (re)
		{
			LogError("SOCKET", "Failed to bind on port: %u", port);
			return;
		}
		re = ::listen(_fd, 5);
		if (re)
		{
			LogError("SOCKET", "Failed to listen on port: %u", port);
			return;
		}
		_open = true;
		// LogSuccess("SOCKET", "Listening on port: %u", port);
		SocketMgr::get_singleton().add(this);
	}
	virtual ~TcpListenSocket()
	{
		this->close();
	}
	virtual void close()
	{	
		if (_open)
		{
			SocketFunc::close_socket(_fd);
			_open = false;
		}
	}
	inline virtual bool is_open() { return _open;}
public:
	virtual void on_accept()
	{
		_temp_fd = accept(_fd, (sockaddr*)&_temp_addr, (socklen_t*)&_addr_size);
		if (_temp_fd == -1)
		{
			return;
		}
		TcpSocket* _temp_socket = new T();
		_temp_socket->accept(&_temp_addr, _temp_fd);
	}
	inline virtual SOCKET get_fd() { return _fd; }

protected:
	SOCKET _fd;
	SOCKET _temp_fd;
	struct sockaddr_in _addr;
	int _port;
	struct sockaddr_in _temp_addr;
	bool _open;
	int _addr_size;
	T* _temp_socket;
};
}
#endif
