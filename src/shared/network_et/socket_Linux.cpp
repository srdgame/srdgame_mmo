/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * SocketLinux.cpp - Linux-specific functions of Socket class are located here.
 *
 */

#include "network.h"
#ifdef CONFIG_USE_EPOLL

void Socket::post_event(uint32 events)
{
    int epoll_fd = sSocketMgr.GetEpollFd();

    struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
    ev.data.fd = m_fd;
    ev.events = events | EPOLLET;			/* use edge-triggered instead of level-triggered because we're using nonblocking sockets */

    // post actual event
    if(epoll_ctl(epoll_fd, EPOLL_CTL_MOD, ev.data.fd, &ev))
		Log.Warning("epoll", "Could not post event on fd %u", m_fd);
}

void Socket::read_callback(uint32 len)
{
    // We have to lock here.
    r_mutex.Acquire();

    size_t space = r_buf.GetSpace();
    int bytes = recv(m_fd, r_buf.get_data(), space, 0);
    if(bytes <= 0)
    {
        r_mutex.Release();
        Disconnect();
        return;
    }    
    else if(bytes > 0)
    {
        //m_readByteCount += bytes;
        r_buf.inc_written(bytes);
        // call virtual onread()
        on_read();
    }

    r_mutex.Release();
}

void Socket::write_callback()
{
    // We should already be locked at this point, so try to push everything out.
    int bytes_written = send(m_fd, w_buf.get_data(), w_buf.GetContiguiousBytes(), 0);
    if(bytes_written < 0)
    {
        // error.
        disconnect();
        return;
    }

    //RemoveWriteBufferBytes(bytes_written, false);
    w_buf.remove(bytes_written);
}

void Socket::burst_push()
{
    if(acquire_send_lock())
        post_event(EPOLLOUT);
}

#endif
