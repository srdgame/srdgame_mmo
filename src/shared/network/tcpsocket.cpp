// tcpsocket.cpp
//

#include "tcpsocket.h"
#include "socketmgr.h"
#include "socketfun.h"
#include <string>
#include "log.h"

using namespace srdgame;

TcpSocket::TcpSocket(size_t rev_buf_size, size_t send_buf_size)
	: Socket(), _rev_buf(rev_buf_size), _send_buf(send_buf_size), _fd(0), _connected(false)
{
	//fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
}

TcpSocket::~TcpSocket()
{
}

bool TcpSocket::connect(const std::string& addr, int port)
{
	// if there is connection left. return false
	if (_connected || _fd != 0)
		return false;

	// create the socket handler.
	// fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); 
	_fd = SocketFunc::create_tcp_socket();

	// get the remote host
	struct hostent * host = ::gethostbyname(addr.c_str());

	if(host == NULL)
		return false;

	_r_addr.sin_family = host->h_addrtype;
	_r_addr.sin_port = ntohs(port);
	::memcpy(&_r_addr.sin_addr.s_addr, host->h_addr_list[0], host->h_length);

	// set it to blocking mode.
	SocketFunc::blocking(_fd);

	if(::connect(_fd, (const sockaddr*)&_r_addr, sizeof(_r_addr)) == -1)
	{
		return false;
	}

	// call events.
	_on_connect();
	return true;

}

bool TcpSocket::accept(sockaddr_in* addr, SOCKET fd)
{
	_fd = fd;
	memcpy(&_r_addr, addr, sizeof(*addr));
	_on_connect();
	return true;
}

bool TcpSocket::close()
{
	_connected = false;

	// remove from mgr
	//SocketMgr::get_instance()->remove(this);
	SocketMgr::get_singleton().remove(this);

	SocketFunc::close_socket(_fd);

	// Call virtual ondisconnect
	on_close();
	return true;
}
/*
bool TcpSocket::is_connectted()
{
	return connected;
}*/

bool TcpSocket::send(const char* data, size_t size)
{
	size_t r_size = 0;
	while (size != 0)
	{
		_send_buf_lock.lock();
		char* buf = _send_buf.reserve(size, r_size);
		memcpy(buf, data, r_size);
		_send_buf.commit(r_size);
		_send_buf_lock.unlock();
		size -= r_size;
		data += r_size;
	}
	return true;
}

bool TcpSocket::send(std::string& data)
{
	return send(data.c_str(), data.length());
}

size_t TcpSocket::rev(char* data, size_t size)
{
	//rev_buf_lock.lock();
	size_t r_size = 0;
	size_t total_size = 0;
	while (size != 0)
	{
		r_size = size;
		_rev_buf_lock.lock();
		char* buf = _rev_buf.get_data(r_size);
		
		if (r_size == 0)
			break;

		memcpy(data, buf, r_size);
		_rev_buf.free(r_size);
		_rev_buf_lock.lock();
		size -= r_size;
		data += r_size;
		total_size += r_size;
	}
	return r_size;
}

std::string TcpSocket::get_remote_ip()
{
	char* ip = (char*)inet_ntoa(_r_addr.sin_addr);
	if(ip != NULL)
	{
		return string( ip );
	}
	else
	{
		return string("");
	}
}

void TcpSocket::_on_connect()
{
	SocketFunc::nonblocking(_fd);
	SocketFunc::disable_buffering(_fd);
	_connected = true;

	SocketMgr::get_singleton().add(this);
	on_connect();
}

void TcpSocket::read_callback(size_t size)
{
	// We have to lock here.
	_rev_buf_lock.lock();
	size_t spare = _rev_buf.spare_size();
	char* buf = _rev_buf.reserve(spare, spare);

	if (buf == NULL || spare == 0)
		return;

	int bytes = recv(_fd, buf, spare, 0);

	LogDebug("SOCKET", "Data received, size is : %d", bytes);


	if(bytes <= 0)
	{
		_rev_buf.commit(0);
		_rev_buf_lock.unlock();
		close();
		return;
	}    
	else if(bytes > 0)
	{
		// for debug:
		char* new_buf = new char[bytes+1];
		::memset(new_buf, 0, bytes + 1);
		::memcpy(new_buf, buf, bytes);
		LogDebug("SOCKET", "Data received, data is : %s", new_buf);
		delete[] new_buf;
		_rev_buf.commit(bytes);
		on_rev();
	}

	_rev_buf_lock.unlock();

}
void TcpSocket::write_callback()
{
	_send_buf_lock.lock();
	int send_size = 0;
	size_t size = 0;
	char* buf = NULL;

	while (true)
	{
		buf = _send_buf.get_data(size);
		if (buf && size)
		{
			send_size += ::send(_fd, buf, size, 0);
		}
		else
		{
			break;
		}
	}
	_send_buf_lock.unlock();
	if (send_size < 0)
	{
		close();
	}
}

void TcpSocket::post_event(int epoll_fd, unsigned int event)
{
	struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
	ev.data.fd = _fd;
	ev.events = event | EPOLLET;			/* use edge-triggered instead of level-triggered because we're using nonblocking sockets */

	// post actual event
	if(epoll_ctl(epoll_fd, EPOLL_CTL_MOD, ev.data.fd, &ev))
	{
	//	Log.Warning("epoll", "Could not post event on fd %u", m_fd);
	}
}
