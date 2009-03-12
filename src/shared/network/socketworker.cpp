#include "socketworker.h"
#include "socket.h"
#include "socketmgr.h"
#include "listensocket.h"
#include "log.h"

using namespace srdgame;

//#define _SocketWorker_DEBUG_

#ifdef _SocketWorker_DEBUG_
#define _LogDebug_ LogDebug
#else
#define _LogDebug_ //
#endif

SocketWorker::SocketWorker()
{
}

SocketWorker::~SocketWorker()
{
	_LogDebug_("SocketWorker", "Destructor of SOcketWorker");
}

bool SocketWorker::run()
{
	_LogDebug_("SocketWorker", "Run Socketworker once *******************");
	int fd_count;
	Socket* ptr = NULL;
	ListenSocket* l_ptr = NULL;
	int i;
	_running = true;
	SocketMgr* mgr = SocketMgr::get_instance();

	while(_running)
	{
		_LogDebug_("SocketWorker", "Start to wait for epoll event");
		fd_count = epoll_wait(mgr->_epoll_fd, _events, THREAD_EVENT_SIZE, 5000);
		_LogDebug_("SocketWorker", "Epoll event coming, count: %d", fd_count);
		if (!_running)
			break;

		for(i = 0; i < fd_count; ++i)
		{
			if(_events[i].data.fd >= SOCKET_HOLDER_SIZE)
			{
				LogWarning("epoll", "Requested FD that is too high (%u)", _events[i].data.fd);
				continue;
			}
			
			ptr = mgr->_fds[_events[i].data.fd];
			
			if(!ptr)
			{
				_LogDebug_("SocketWorker", "Accepting one new socket one listen  fd :%d", _events[i].data.fd);
				l_ptr = mgr->_listen_fds[_events[i].data.fd];
				if(l_ptr)
				{
					l_ptr->on_accept();
				}
				else
				{
					// TODO:
					LogWarning("epoll", "Returned invalid fd (no pointer) of FD %u", _events[i].data.fd);
				}
				_LogDebug_("SocketWorker", "Accepting complete!!!");
				continue;
			}

			if(_events[i].events & EPOLLHUP || _events[i].events & EPOLLERR)
			{
				_LogDebug_("SocketWorker", "One socket is shutdown. fd: %d", _events[i].data.fd);
				ptr->close();
				ptr->_delete();
				_LogDebug_("SocketWorker", "Closing complete!");
				continue;
			}
			if(_events[i].events & EPOLLIN)
			{
				_LogDebug_("SocketWorker", "Data coming, call read_callback");
				ptr->read_callback(0);               // Len is unknown at this point.
				_LogDebug_("SocketWorker", "Data processing complete!");
				
				/* changing to written state? */
				if(ptr->is_connected())
				{
					//ptr->post_event(EPOLLOUT);
				}
				else
				{
					// It is time to delete it, since no longer need this connection
					ptr->_delete();

					continue;	
				}
			
			}
			if(_events[i].events & EPOLLOUT)
			{
				_LogDebug_("SocketWorker", "Data is willing to send, call write_callback()");
				ptr->write_callback();       // Perform actual send()
				_LogDebug_("SocketWorker", "Data processing complete!");
				if (ptr->is_connected())
				{
					/* change back to a read event */
					ptr->post_event(EPOLLIN);
				}
				else
				{
					ptr->_delete();
					continue;
				}
			}
		}       
	}
	return true;
}

void SocketWorker::on_close()
{
}

