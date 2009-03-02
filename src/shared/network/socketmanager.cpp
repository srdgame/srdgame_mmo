#include "socketmanager.h"
#include "socket.h"
#include <cstdlib>
#include "threadpool.h"
#include "socketworker.h"
#include "networkconfig.h"

using namespace srdgame;

Mutex SocketManager::_lock;

SocketManager::SocketManager() : _count(0), _epoll_fd(0), _max_fd(0)
{
	_lock.lock();
	_epoll_fd = epoll_create(SOCKET_HOLDER_SIZE);
	if (-1 == _epoll_fd)
	{
		// Output debug info.
		exit(-1);
	}
	memset(_fds, 0, SOCKET_HOLDER_SIZE);
	_lock.unlock();
}

SocketManager::~SocketManager()
{
	close(_epoll_fd);
}

void SocketManager::add(Socket* s)
{
	if (!NetworkConfig::get_singleton().socket_check(s))
	{
		s->close();
		return;
	}
	if (_fds[s->get_fd()] != NULL)
	{
		// the connection is existing.
		s->close();
		return;
	}
	if (_max_fd < s->get_fd())
	{
		_max_fd = s->get_fd();
	}

	_lock.lock();
	_fds[s->get_fd()] = s; 
	struct epoll_event ev;
    	memset(&ev, 0, sizeof(epoll_event));
    	ev.events = (NULL != s->get_rev_buf()) ? EPOLLOUT : EPOLLIN;
    	ev.events |= EPOLLET;			/* use edge-triggered instead of level-triggered because we're using nonblocking sockets */
    	ev.data.fd = s->get_fd();
    
    	if(0 != epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, ev.data.fd, &ev))
	{
		//Log.Warning("epoll", "Could not add event to epoll set on fd %u", ev.data.fd);
		// TODO: output debug.  and quit?
	}
	// set the _count.
	++_count;
	_lock.unlock();
}
void SocketManager::remove(Socket* s)
{
	_lock.lock();
	if (_fds[s->get_fd()] != s)
	{
		// TODO: output debug
		_lock.unlock();
		return;
	}

    	// Remove from epoll list.
    	struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
    	ev.data.fd = s->get_fd();
    	ev.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP | EPOLLONESHOT;

	if(epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, ev.data.fd, &ev))
	{
		//Log.Warning("epoll", "Could not remove fd %u from epoll set, errno %u", s->get_fd(), errno);
	}
	
    	_fds[s->get_fd()] = 0;
	--_count;
	_lock.unlock();
}
void SocketManager::close_all()
{
	_lock.lock();
	for (size_t i = 0; i < SOCKET_HOLDER_SIZE; ++i)
	{
		if (_fds[i] != NULL)
		{
			_fds[i]->close();
		}
	}
	_lock.unlock();
}
void SocketManager::show_info()
{
	// TODO: show what?
}
void SocketManager::start_worker()
{
	ThreadPool::get_singleton().execute(new SocketWorker());
}

