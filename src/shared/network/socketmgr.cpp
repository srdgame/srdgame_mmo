#include "socketmgr.h"
#include "socket.h"
#include "listensocket.h"
#include <cstdlib>
#include "threadpool.h"
#include "socketworker.h"
#include "networkconfig.h"
#include "log.h"

using namespace srdgame;

//Mutex SocketMgr::_lock;

SocketMgr::SocketMgr() : _count(0), _epoll_fd(0), _max_fd(0)
{
#ifndef SOCKET_MGR_NO_NEED_LOCK
	// Lock the data, and create epoll and initialize the array.
	_lock.lock();
#endif
	_epoll_fd = epoll_create(SOCKET_HOLDER_SIZE);
	if (-1 == _epoll_fd)
	{
		// Output debug info.
		LogError("SOCKET", "Could not create epoll fd");

#ifndef SOCKET_MGR_NO_NEED_LOCK
		_lock.unlock();
#endif

		exit(-1);
	}
	// Set to clean.
	memset(_fds, 0, SOCKET_HOLDER_SIZE);
	memset(_listen_fds, 0, SOCKET_LISTEN_HOLDER_SIZE);

#ifndef SOCKET_MGR_NO_NEED_LOCK
	_lock.unlock();
#endif
}

SocketMgr::~SocketMgr()
{
#ifndef SOCKET_MGR_NO_NEED_LOCK
	_lock.lock();
#endif	
	// Close the epoll.
	close(_epoll_fd);
	// Do not call close all, or you will see it is hard to close it... And seems we do not need to close them
	//close_all();
#ifndef SOCKET_MGR_NO_NEED_LOCK
	_lock.unlock();
#endif
}

void SocketMgr::add(Socket* s)
{
	if (!NetworkConfig::get_singleton().socket_check(s))
	{
		s->close();
		return;
	}
#ifndef SOCKET_MGR_NO_NEED_LOCK
	// Lock the data
	_lock.lock();
#endif

	// I am not sure whether the _fds is enough for us or not. It is better to use std::map.
	// TODO:
	if (_fds[s->get_fd()] != NULL)
	{
#ifndef SOCKET_MGR_NO_NEED_LOCK
		_lock.unlock();
#endif
		// the connection is existing.
		LogWarning("SOCKET", "The connection is existing about fd: %d", s->get_fd());
		s->close();
		return;
	}

	// get the max file d...?
	if (_max_fd < s->get_fd())
	{
		_max_fd = s->get_fd();
	}
	// Save the value.
	_fds[s->get_fd()] = s; 
	struct epoll_event ev;
    	memset(&ev, 0, sizeof(epoll_event));
    	ev.events = (NULL != s->get_rev_buf()) ? EPOLLOUT : EPOLLIN;
    	ev.events |= EPOLLET;			/* use edge-triggered instead of level-triggered because we're using nonblocking sockets */
    	ev.data.fd = s->get_fd();
    
    	if(0 != epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, ev.data.fd, &ev))
	{
		LogWarning("SOCKET", "Could not add event to epoll set on fd %u", ev.data.fd);
		// TODO: output debug.  and quit?
	}
	// set the _count.
	++_count;
#ifndef SOCKET_MGR_NO_NEED_LOCK
	_lock.unlock();
#endif
}
void SocketMgr::add(ListenSocket* s)
{
	LogDebug("SOCKET", "Adding listening socket : %d", s->get_fd());
#ifndef SOCKET_MGR_NO_NEED_LOCK
	_lock.lock();
#endif

	ASSERT(_listen_fds[s->get_fd()] == 0);
	_listen_fds[s->get_fd()] = s;
	
	struct epoll_event event;
	::memset(&event, 0, sizeof(epoll_event));
	event.events = EPOLLIN | EPOLLET;
	event.data.fd = s->get_fd();
	
	// control
	if (::epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, event.data.fd, &event))
	{
		LogError("SOCKET", "Could not add to epoll event set on fd: %u", event.data.fd);
	}
#ifndef SOCKET_MGR_NO_NEED_LOCK
	_lock.unlock();
#endif
}
void SocketMgr::remove(Socket* s)
{
#ifndef SOCKET_MGR_NO_NEED_LOCK	
	_lock.lock();
#endif
	if (_fds[s->get_fd()] != s)
	{
		// TODO: output debug
		LogWarning("SOCKET", "Could not remove socket on fd: %u", s->get_fd()); 
#ifndef SOCKET_MGR_NO_NEED_LOCK
		_lock.unlock();
#endif
		return;
	}

    	// Remove from epoll list.
    	struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
    	ev.data.fd = s->get_fd();
    	ev.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP | EPOLLONESHOT;

	if(epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, ev.data.fd, &ev))
	{
		//LogWarning("SOCKET", "Could not remove fd %u from epoll set, errno %u", s->get_fd(), errno);
		LogWarning("SOCKET", "Could not remove fd %u from epoll set", s->get_fd());

	}
	
    	_fds[s->get_fd()] = 0;
	--_count;
#ifndef SOCKET_MGR_NO_NEED_LOCK	
	_lock.unlock();
#endif
}
void SocketMgr::close_all()
{
#ifndef SOCKET_MGR_NO_NEED_LOCK	
	_lock.lock();
#endif
	for (size_t i = 0; i < SOCKET_HOLDER_SIZE; ++i)
	{
		if (_fds[i] != NULL)
		{
			_fds[i]->close();
		}
	}
#ifndef SOCKET_MGR_NO_NEED_LOCK	
	_lock.unlock();
#endif
}
void SocketMgr::show_info()
{
	// TODO: show what?
}
void SocketMgr::start_worker()
{
	ThreadPool::get_singleton().execute(new SocketWorker());
}

