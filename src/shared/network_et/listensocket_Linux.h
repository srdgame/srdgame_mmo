/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * ListenSocket<T>: Creates a socket listener on specified addr and port,
 *                  requires Update() to be called every loop.
 *
*/

#ifndef _LISTENSOCKET_H
#define _LISTENSOCKET_H
#ifdef CONFIG_USE_EPOLL

#include "socketdefs.h"
#include <errno.h>

class ListenSocketBase
{
public:
	virtual ~ListenSocketBase() {}
	virtual void on_accept() = 0;
	virtual int get_fd() = 0;
};

template<class T>
class ListenSocket : public ListenSocketBase
{
public:
	ListenSocket(const char * listen_addr, size_t Port) : ListenSocketBase()
    {
        socket = socket(AF_INET, SOCK_STREAM, 0);
        SocketOps::reuse_addr(socket);
        SocketOps::nonblocking(socket);
        SocketOps::set_timeout(socket, 60);

        addr.sin_family = AF_INET;
        addr.sin_port = ntohs((u_short)Port);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        opened = false;

        if(strcmp(listen_addr, "0.0.0.0"))
        {
            struct hostent * hostname = gethostbyname(listen_addr);
            if(hostname != 0)
                memcpy(&addr.sin_addr.s_addr, hostname->h_addr_list[0], hostname->h_length);
        }

        // bind.. well attempt to.
        int ret = bind(socket, (const sockaddr*)&addr, sizeof(addr));
        if(ret != 0)
        {
            printf("Bind unsuccessful on port %u.", (unsigned int)Port);
            return;
        }

        ret = listen(socket, 5);
        if(ret != 0) 
        {
            printf("Unable to listen on port %u.", (unsigned int)Port);
            return;
        }
        len = sizeof(sockaddr_in);
        opened = true;
		sSocketMgr.add_listen_socket(this);
    }

    ~ListenSocket()
    {
        if(opened)
            SocketOps::close_socket(socket);
    }

    void Close()
    {
        if(opened)
            SocketOps::close_socket(socket);
        opened = false;
    }

    /*void Update()
    {
        aSocket = accept(socket, (sockaddr*)&temp_addr, (socklen_t*)&len);
        if(aSocket == -1)
            return;

        dsocket = new T(aSocket);
        dsocket->Accept(&temp_addr);
    }*/

	void on_accept()
	{
		aSocket = accept(socket, (sockaddr*)&temp_addr, (socklen_t*)&len);
		if(aSocket == -1)
			return;

		dsocket = new T(aSocket);
		dsocket->Accept(&temp_addr);
	}

    inline bool is_open() { return opened; }
	int get_fd() { return socket; }	

private:
    SOCKET socket;
    SOCKET aSocket;
    struct sockaddr_in addr;
    struct sockaddr_in temp_addr;
    bool opened;
    size_t len;
    T * dsocket;
};

#endif
#endif

