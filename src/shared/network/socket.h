/**============================================================================
 * Copyright (C) 2009 SrdGame Team
 *
 * File: scoket.h
 * Description:
        This file is to define socket base class.
 * Author: cch
 * Update: 2009-2-27
 * Verison: 0.0.1
=============================================================================*/

#ifndef SOCKET_H_
#define SOCKET_H_

#include "bufferbase.h"
#include "socketdefs.h"
#include <string>

namespace srdgame
{
class Socket
{
public:
	// Create socket to connect to other pc,
	virtual ~Socket(){};

	// method?
	virtual bool accept(sockaddr_in* addr, SOCKET fd) = 0;
	virtual bool connect(const std::string& addr, int port) = 0;
	// close/disconnect
	virtual bool close() = 0;
	// Delete the object
	virtual void _delete() = 0;
	// whether the connection still there.
	virtual bool is_connected() = 0;
	// send data out.
	virtual bool send(const char* data, size_t size) = 0;
	virtual bool send(std::string& data) = 0;
	// access the buffer directly, if you donot want to create the buffer by yourself. and you must lock the receive buffer during your time.
	virtual BufferBase* get_rev_buf() = 0;
	// prepare your buffer first, then call this to get data.
	virtual size_t rev(char* data, size_t size) = 0;

	// attributes
	virtual std::string get_remote_ip() = 0;
	virtual int get_remote_port() = 0;
	
	virtual void lock_rev_buf() = 0;
	virtual void unlock_rev_buf() = 0;

	virtual SOCKET get_fd() = 0;

protected:
	// events.
	virtual void on_rev() = 0;
	virtual void on_send() = 0;
	virtual void on_connect() = 0;
	virtual void on_close() = 0;

protected:
	friend class SocketWorker;
	virtual void read_callback(size_t size) = 0;
	virtual void write_callback() = 0;
	virtual void post_event(unsigned int event) = 0;
};
}

#endif
