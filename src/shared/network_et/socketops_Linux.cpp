/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * SocketOpsLinux.cpp - Linux implementation of SocketOps.
 *
 */


#include "network.h"
#ifdef CONFIG_USE_EPOLL

namespace SocketOps
{
    // Create file descriptor for socket i/o operations.
    SOCKET create_tcp_file_descriptor()
    {
        // create a socket for use with overlapped i/o.
        return socket(AF_INET, SOCK_STREAM, 0);
    }

    // Disable blocking send/recv calls.
    bool nonblocking(SOCKET fd)
    {
        size_t arg = 1;
        return (::ioctl(fd, FIONBIO, &arg) == 0);
    }

    // Disable blocking send/recv calls.
    bool blocking(SOCKET fd)
    {
        size_t arg = 0;
        return (ioctl(fd, FIONBIO, &arg) == 0);
    }

    // Disable nagle buffering algorithm
    bool disable_buffering(SOCKET fd)
    {
        size_t arg = 1;
        return (setsockopt(fd, 0x6, 0x1, (const char*)&arg, sizeof(arg)) == 0);
    }

    // Enable nagle buffering algorithm
    bool enable_buffering(SOCKET fd)
    {
        size_t arg = 0;
        return (setsockopt(fd, 0x6, 0x1, (const char*)&arg, sizeof(arg)) == 0);
    }

    // Set internal buffer size to socket.
    bool set_send_buf_size(SOCKET fd, size_t size)
    {
        return (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const char*)&size, sizeof(size)) == 0);
    }

    // Set internal buffer size to socket.
    bool set_recv_buf_size(SOCKET fd, size_t size)
    {
        return (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const char*)&size, sizeof(size)) == 0);
    }

	// Set internal timeout.
	bool set_timeout(SOCKET fd, size_t timeout)
	{
		struct timeval to;
		to.tv_sec = timeout;
		to.tv_usec = 0;
		if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&to, (socklen_t)sizeof(to)) != 0) return false;
		return (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&to, (socklen_t)sizeof(to)) == 0);
	}

    // Closes a socket fully.
    void close_socket(SOCKET fd)
    {
        shutdown(fd, SHUT_RDWR);
        close(fd);
    }

    // Sets reuseaddr
    void reuse_addr(SOCKET fd)
    {
        size_t option = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, 4);
    }
}

#endif
