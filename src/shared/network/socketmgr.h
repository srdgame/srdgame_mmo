#ifndef SOCKET_MANAGER_H_
#define SOCKET_MANAGER_H_

#include "singleton.h"
#include "socketdefs.h"
#include "mutex.h"
#include "autolock.h"

namespace srdgame
{
#define SOCKET_HOLDER_SIZE 30000    // You don't want this number to be too big, otherwise you're gonna be eating
                                    // memory. 65536 = 256KB, so thats no big issue for now, and I really can't
                                    // see anyone wanting to have more than 65536 concurrent connections.

#define SOCKET_LISTEN_HOLDER_SIZE 32 // should be much more than you need.

#define SOCKET_MGR_NO_NEED_LOCK // No need to have lock here, since all the sockets are started by the worker and will delete and used by the worker only... So lock is useless.  

class Socket;
class ListenSocket;
class SocketMgr : public Singleton<SocketMgr>
{
	friend class SocketWorker;
public:
	SocketMgr();
	~SocketMgr();
public:
	// Add socket to be here to be managed.
	void add(Socket* s);
	void add(ListenSocket* s);

	// Remove the socket from here.
	void remove(Socket* s);

	// Get socket count, do't include listen sockets.
	inline size_t count()
	{
#ifndef SOCKET_MGR_NO_NEED_LOCK
		AutoLock l
}ock(_lock);
#endif
		return _count;
	}

	//  Close all the sockets.
	void close_all();

	void show_info();

	// Should call this to start the worker thread, or all the sockets are not working.
	void start_worker();

	int get_epoll_fd() 
	{
#ifndef SOCKET_MGR_NO_NEED_LOCK
		AutoLock lock(_lock);
#endif
		return _epoll_fd;
	}
protected:	
	size_t _count;
	
    	int _epoll_fd;

    	Socket * _fds[SOCKET_HOLDER_SIZE];
	ListenSocket * _listen_fds[SOCKET_LISTEN_HOLDER_SIZE];

	SOCKET _max_fd;
#ifndef SOCKET_MGR_NO_NEED_LOCK
	Mutex _lock;
#endif
};
//SocketMgr& pMgr = SocketMgr::get_singleton();
}
#endif
