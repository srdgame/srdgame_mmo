/**============================================================================
 * Copyright (C) 2009 SrdGame Team
 *
 * File: tcpscoket.h
 * Description:
        This file is to define socket base class.
 * Author: cch
 * Update: 2009-2-27
 * Verison: 0.0.1
=============================================================================*/

#ifndef TCP_SOCKET_H_
#define TCP_SOCKET_H_

#include "socket.h"
#include "socketdefs.h"
#include "bipbuffer.h"
#include <string>
#include "mutex.h"
#include "rwlock.h"

namespace srdgame
{
class TcpSocket : public Socket
{
public:
	// Create socket to connect to other pc,
	TcpSocket(size_t rev_buf_size = SYS_PAGE_SIZE, size_t send_buf_size = SYS_PAGE_SIZE);

	// 
	virtual ~TcpSocket();

	// method?
	virtual bool connect(const std::string& addr, int port);
	// accept connection
	virtual bool accept(sockaddr_in* addr, SOCKET fd);
	// close/disconnect
	virtual bool close();
	// whether the connection still there.
	inline virtual bool is_connected()
	{
		return _connected;
	}
	// send data out.
	virtual bool send(const char* data, size_t size);
	virtual bool send(std::string& data);
	// access the buffer directly, if you donot want to create the buffer by yourself.
	inline virtual BufferBase* get_rev_buf()
	{
		return &_rev_buf;
	}
	// prepare your buffer first, then call this to get data.
	virtual size_t rev(char* data, size_t size);

	// attributes
	virtual std::string get_remote_ip();
	inline virtual int get_remote_port()
	{
		return ntohs(_r_addr.sin_port); 
	}
	
	inline virtual void lock_rev_buf()
	{
		_rev_buf_lock.lock();
	}
	inline virtual void unlock_rev_buf()
	{
		_rev_buf_lock.unlock();
	}

	inline virtual SOCKET get_fd()
	{
		return _fd;
	}

protected:
	// events.
	virtual void on_rev() = 0;
	virtual void on_send() = 0;
	virtual void on_connect() = 0;
	virtual void on_close() = 0;

protected:
	// helper functions
	void _on_connect();

protected:
	virtual void read_callback(size_t size);
	virtual void write_callback();
	virtual void post_event(int epoll_fd, unsigned int event);

protected:
	BipBuffer _rev_buf;
	Mutex _rev_buf_lock;
	BipBuffer _send_buf;
	Mutex _send_buf_lock;

	// attributes
	Mutex _socket_lock;
	SOCKET _fd;
	sockaddr_in _r_addr;
	bool _connected;
};
}

#endif
