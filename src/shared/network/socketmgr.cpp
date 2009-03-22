#include "socketmgr.h"
#include "socket.h"
#include "listensocket.h"
#include <cstdlib>
#include <errno.h>
#include "threadpool.h"
#include "socketworker.h"
#include "networkconfig.h"
#include "log.h"

using namespace srdgame;

//Mutex SocketMgr::_lock;

SocketMgr::SocketMgr() : _count(0), _epoll_fd(0), _max_fd(0)
{
	_epoll_fd = epoll_create(SOCKET_HOLDER_SIZE);
	if (-1 == _epoll_fd)
	{
		// Output debug info.
		LogError("SOCKET", "Could not create epoll fd");

		exit(-1);
	}
	// Set to clean.
	memset(_fds, 0, SOCKET_HOLDER_SIZE * sizeof(Socket*));
	memset(_listen_fds, 0, SOCKET_HOLDER_SIZE * sizeof(ListenSocket*) );
	LogDebug("SOCKET", "Size of _fds is : %d", sizeof(_fds));
}

SocketMgr::~SocketMgr()
{
	LogDebug("SOCKET", "Destructor of SOcketMgr");
	// Close the epoll.
	close(_epoll_fd);
	LogDebug("SOCKET", "Closed the epool fd");
	// Do not call close all, or you will see it is hard to close it... And seems we do not need to close them
	close_all();
	LogDebug("SOCKET", "CLOSED all the connections");
}

void SocketMgr::add(Socket* s)
{
	if (!NetworkConfig::get_singleton().socket_check(s))
	{
		s->close();
		return;
	}

	// I am not sure whether the _fds is enough for us or not. It is better to use std::map.
	// TODO:
	if (_fds[s->get_fd()])
	{
		// the connection is existing.
		LogWarning("SOCKET", "The connection is existing about fd: %d", s->get_fd());
		s->close();
		return;
	}

	// get the max file d...?
	if (_max_fd < s->get_fd())
	{
		_max_fd = s->get_fd();
		if (_max_fd >= SOCKET_HOLDER_SIZE)
		{
			LogError("SOCKET", "Socket fd is too big than what we can hold, max_fd : %d", _max_fd);
			return;
		}
	}
	// Save the value.
	_fds[s->get_fd()] = s;

	// Register event
	struct epoll_event ev;
    	memset(&ev, 0, sizeof(epoll_event));
    	ev.events = EPOLLIN;
    	ev.events |= EPOLLET;			/* use edge-triggered instead of level-triggered because we're using nonblocking sockets */
    	ev.data.fd = s->get_fd();
    
    	if(0 != epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, ev.data.fd, &ev))
	{
		LogWarning("SOCKET", "Could not add event to epoll set on fd %u", ev.data.fd);
		// TODO: output debug.  and quit?
	}
	// set the _count.
	++_count;
}
void SocketMgr::add(ListenSocket* s)
{
	LogDebug("SOCKET", "Adding listening socket : %d", s->get_fd());

	ASSERT(!_listen_fds[s->get_fd()]);
	ASSERT(s->get_fd() < SOCKET_HOLDER_SIZE);

	_listen_fds[s->get_fd()] = s;
	
	struct epoll_event event;
	::memset(&event, 0, sizeof(epoll_event));
	event.events = EPOLLIN | EPOLLOUT;
	event.events = event.events | EPOLLET;
	event.data.fd = s->get_fd();
	
	// control
	if (::epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, event.data.fd, &event))
	{
		LogError("SOCKET", "Could not add to epoll event set on fd: %u", event.data.fd);
	}
}
void SocketMgr::remove(Socket* s)
{
	if (!_fds[s->get_fd()])
	{
		LogDebug("SOCKET", "Already removed the socket on fd: %u", s->get_fd());
		return;
	}
	if (_fds[s->get_fd()] != s)
	{
		// TODO: output debug
		LogWarning("SOCKET", "Could not remove socket on fd: %u", s->get_fd()); 
		return;
	}

    	// Remove from epoll list.
    	struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
    	ev.data.fd = s->get_fd();
    	ev.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP | EPOLLONESHOT;

	if(epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, ev.data.fd, &ev))
	{
		LogWarning("SOCKET", "Could not remove fd %u from epoll set, errno %u", s->get_fd(), errno);
		//LogWarning("SOCKET", "Could not remove fd %u from epoll set", s->get_fd());

	}
	else
	{
		LogSuccess("SOCKET", "Removed fd %u from epoll set", s->get_fd());
	}
	
    	_fds[s->get_fd()] = NULL;
	--_count;
}
void SocketMgr::close_all()
{
	for (size_t i = 0; i < SOCKET_HOLDER_SIZE; ++i)
	{
		if (_fds[i])
		{
			Socket* ptr = _fds[i];
			ptr->close();
			ptr->_delete();
		//	_fds[i] = NULL;  the close will remove itself.
		}
	}
}
void SocketMgr::show_info()
{
	// TODO: show what?
}
void SocketMgr::start_worker()
{
	ThreadPool::get_singleton().execute(new SocketWorker());
}

