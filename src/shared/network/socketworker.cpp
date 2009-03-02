#include "socketworker.h"
#include "socket.h"
#include "socketmanager.h"
#include "listensocket.h"

using namespace srdgame;

SocketWorker::SocketWorker()
{
}

SocketWorker::~SocketWorker()
{
}

bool SocketWorker::run()
{
    int fd_count;
    Socket * ptr;
    int i;
    _running = true;
    SocketManager* mgr = SocketManager::get_instance();

    while(_running)
    {
        fd_count = epoll_wait(mgr->_epoll_fd, _events, THREAD_EVENT_SIZE, 5000);
        for(i = 0; i < fd_count; ++i)
        {
            if(_events[i].data.fd >= SOCKET_HOLDER_SIZE)
            {
		    //TODO:
		    //Log.Warning("epoll", "Requested FD that is too high (%u)", events[i].data.fd);
		    continue;
            }

            ptr = mgr->_fds[_events[i].data.fd];

            if(ptr == NULL)
            {
		    if( (ptr = ((Socket*)mgr->_listen_fds[_events[i].data.fd])) != NULL )
		    {
			    ((ListenSocket*)ptr)->on_accept();
		    }
		    else
		    {
			    // TODO:
			    //Log.Warning("epoll", "Returned invalid fd (no pointer) of FD %u", events[i].data.fd);
		    }
		    continue;
            }

            if(_events[i].events & EPOLLHUP || _events[i].events & EPOLLERR)
            {
		    ptr->close();
		    continue;
            }
	    else if(_events[i].events & EPOLLIN)
            {
		    ptr->read_callback(0);               // Len is unknown at this point.

		    /* changing to written state? */
		    if(ptr->get_rev_buf() && ptr->is_connected())
		    {
			    ptr->post_event(mgr->_epoll_fd, EPOLLOUT);
		    }
            }
	    else if(_events[i].events & EPOLLOUT)
            {
		    ptr->write_callback();       // Perform actual send()
		    if(ptr->get_rev_buf() == NULL)
		    {
                    	/* we don't have to do anything here. no more oneshots :) */
		    }
		    else
		    {
			    /* change back to a read event */
			    ptr->post_event(mgr->_epoll_fd, EPOLLIN);
		    }
	    }
	}       
    }
    return true;
}

void SocketWorker::on_close()
{
}
