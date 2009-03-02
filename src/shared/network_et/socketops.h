/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * SocketOps - wrapper for any specific socket operations that may be platform-dependant.
 *
 */


#ifndef SOCKET_OPS_H
#define SOCKET_OPS_H

namespace SocketOps
{
	// Create file descriptor for socket i/o operations.
	SOCKET create_tcp_file_descriptor();

	// Disable blocking send/recv calls.
	bool nonblocking(SOCKET fd);

	// Enable blocking send/recv calls.
	bool blocking(SOCKET fd);

	// Disable nagle buffering algorithm
	bool disable_buffering(SOCKET fd);

	// Enables nagle buffering algorithm
	bool enable_buffering(SOCKET fd);

	// set_ internal buffer size to socket.
	bool set_recv_buffer_size(SOCKET fd, size_t size);

	// set_ internal buffer size to socket.
	bool set_send_buf_size(SOCKET fd, size_t size);

	// set_ timeout, in seconds
	bool set_timeout(SOCKET fd, size_t timeout);

	// Closes socket completely.
	void close_socket(SOCKET fd);

	// set_s SO_REUSEADDR
	void reuse_addr(SOCKET fd);
};

#endif

