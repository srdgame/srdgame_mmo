/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * Socket implementable class.
 *
 */

#include "network.h"


Socket::Socket(SOCKET fd, size_t sendbuffersize, size_t recvbuffersize) : fd(fd), connected(false),	deleted(false)
{
	// alloc Buffers
	r_buf_.alloc(recvbuffersize);
	w_buf_.alloc(sendbuffersize);

	// IOCP Member Variables
#ifdef CONFIG_USE_IOCP
	write_lock = 0;
	completion_port = 0;
#else
	write_lock = 0;
#endif

	// Check for needed fd allocation.
	if(fd == 0)
		fd = SocketOps::CreateTCPFileDescriptor();
}

Socket::~Socket()
{

}

bool Socket::connect(const char * Address, size_t Port)
{
	struct hostent * ci = gethostbyname(Address);
	if(ci == 0)
		return false;

	client.sin_family = ci->h_addrtype;
	client.sin_port = ntohs((u_short)Port);
	memcpy(&client.sin_addr.s_addr, ci->h_addr_list[0], ci->h_length);

	SocketOps::blocking(fd);
	if(connect(fd, (const sockaddr*)&client, sizeof(client)) == -1)
		return false;

	// at this point the connection was established
#ifdef CONFIG_USE_IOCP
	completion_port = sSocketMgr.get_completion_port();
#endif
	on_connect();
	return true;
}

void Socket::accept(sockaddr_in * address)
{
	memcpy(&client, address, sizeof(*address));
	on_connect();
}

void Socket::on_connect()
{
	// set common parameters on the file descriptor
	SocketOps::nonblocking(fd);
	SocketOps::disable_buffering(fd);
/*	SocketOps::SetRecvBuffersize(fd, w_buf_size);
	SocketOps::SetsendBuffersize(fd, w_buf_size);*/
	connected = true;

	// IOCP stuff
#ifdef CONFIG_USE_IOCP
	assign_to_completion_port();
	setup_read_event();
#endif
	sSocketMgr.add_socket(this);

	// Call virtual onconnect
	on_connect();
}

bool Socket::send(const char * data, size_t size)
{
	bool rv;

	// This is really just a wrapper for all the burst stuff.
	burst_begin();
	rv = burst_send(data, size);
	if(rv)
		burst_push();
	burst_end();

	return rv;
}

bool Socket::burst_send(const char * data, size_t size)
{
	return w_buf_.write(data, size);
}

string Socket::get_remote_IP()
{
	char* ip = (char*)inet_ntoa( client.sin_addr );
	if( ip != NULL )
		return string( ip );
	else
		return string( "noip" );
}

void Socket::disconnect()
{
	connected = false;

	// remove from mgr
	sSocketMgr.remove_socket(this);

	SocketOps::close_socket(fd);

	// Call virtual ondisconnect
	on_disconnect();

	if(!deleted) delete();
}

void Socket::delete()
{
	if(deleted) return;
	deleted = true;

	if(connected) disconnect();
	sSocketGarbageCollector.queue_socket(this);
}

