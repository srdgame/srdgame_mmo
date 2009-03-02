/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * SocketMgr - epoll manager for Linux.
 *
 */

#include "network.h"
#ifdef CONFIG_USE_EPOLL

//#define ENABLE_ANTI_DOS

void SocketMgr::add_socket(Socket * s)
{
#ifdef ENABLE_ANTI_DOS
	size_t saddr;
	int i, count;

	// Check how many connections we already have from that ip
	saddr = s->get_remote_addr().s_addr;
	for (i=0, count=0; i<=max_fd; i++)
	{
		if (fds[i])
		{
			if (fds[i]->get_remote_addr().s_addr == saddr) count++;
		}
	}

	// More than 16 connections from the same ip? enough! xD
	if (count > 16)
	{
		s->disconnect(false);
		return;
	}
#endif

	if(fds[s->get_fd()] != NULL)
	{
		//fds[s->get_fd()]->Delete();
		//fds[s->get_fd()] = NULL;
		s->delete();
		return;
	}

	if (max_fd < s->get_fd()) max_fd = s->get_fd();
    fds[s->get_fd()] = s;

    // Add epoll event based on socket activity.
    struct epoll_event ev;
    memset(&ev, 0, sizeof(epoll_event));
    ev.events = (s->GetWriteBuffer().GetSize()) ? EPOLLOUT : EPOLLIN;
    ev.events |= EPOLLET;			/* use edge-triggered instead of level-triggered because we're using nonblocking sockets */
    ev.data.fd = s->get_fd();
    
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, ev.data.fd, &ev))
		Log.Warning("epoll", "Could not add event to epoll set on fd %u", ev.data.fd);
}

void SocketMgr::AddListenSocket(ListenSocketBase * s)
{
	assert(listenfds[s->get_fd()] == 0);
	listenfds[s->get_fd()] = s;

	// Add epoll event based on socket activity.
	struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
	ev.events = EPOLLIN;
	ev.events |= EPOLLET;			/* use edge-triggered instead of level-triggered because we're using nonblocking sockets */
	ev.data.fd = s->get_fd();

	if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, ev.data.fd, &ev))
		Log.Warning("epoll", "Could not add event to epoll set on fd %u", ev.data.fd);
}

void SocketMgr::RemoveSocket(Socket * s)
{
    if(fds[s->get_fd()] != s)
	{
		Log.Warning("epoll", "Could not remove fd %u from the set due to it not existing?", s->get_fd());
        return;
	}

	fds[s->get_fd()] = 0;
    // Remove from epoll list.
    struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
    ev.data.fd = s->get_fd();
    ev.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP | EPOLLONESHOT;

    if(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, ev.data.fd, &ev))
		Log.Warning("epoll", "Could not remove fd %u from epoll set, errno %u", s->get_fd(), errno);
}

void SocketMgr::close_all()
{
    for(size_t i = 0; i < SOCKET_HOLDER_SIZE; ++i)
        if(fds[i] != NULL)
            fds[i]->Delete();
}

void SocketMgr::spawn_worker_threads()
{
    size_t count = 1;
    for(size_t i = 0; i < count; ++i)
        ThreadPool.execute_task(new SocketWorkerThread());
}

void SocketMgr::show_status()
{
	int i, count;

	for (i=0, count=0; i<=max_fd; i++)
	{
		if (fds[i]) count++;
	}

	sLog.out_string("Sockets: %d", count);
}

bool SocketWorkerThread::run()
{
    int fd_count;
    Socket * ptr;
    int i;
    running = true;
    SocketMgr * mgr = SocketMgr::get_instance();

    while(running)
    {
        fd_count = epoll_wait(mgr->epoll_fd, events, THREAD_EVENT_SIZE, 5000);
        for(i = 0; i < fd_count; ++i)
        {
            if(events[i].data.fd >= SOCKET_HOLDER_SIZE)
            {
                Log.warning("epoll", "Requested FD that is too high (%u)", events[i].data.fd);
                continue;
            }

            ptr = mgr->fds[events[i].data.fd];

            if(ptr == NULL)
            {
				if( (ptr = ((Socket*)mgr->listenfds[events[i].data.fd])) != NULL )
					((ListenSocketBase*)ptr)->on_accept();
				else
					Log.warning("epoll", "Returned invalid fd (no pointer) of FD %u", events[i].data.fd);

                continue;
            }

            if(events[i].events & EPOLLHUP || events[i].events & EPOLLERR)
            {
				ptr->disconnect();
                continue;
            }
			else if(events[i].events & EPOLLIN)
            {
                ptr->read_callback(0);               // Len is unknown at this point.

				/* changing to written state? */
				if(ptr->get_write_buf().GetSize() && !ptr->has_send_lock() && ptr->is_connected())
					ptr->post_event(EPOLLOUT);
            }
			else if(events[i].events & EPOLLOUT)
            {
                ptr->burst_begin();          // Lock receive mutex
                ptr->write_callback();       // Perform actual send()
                if(ptr->get_write_buf().get_size() > 0)
                {
                    /* we don't have to do anything here. no more oneshots :) */
                }
                else
                {
					/* change back to a read event */
                    ptr->dec_send_lock();
                    ptr->post_event(EPOLLIN);
                }
                ptr->burst_end();            // Unlock
            }
        }       
    }
    return true;
}

#endif
