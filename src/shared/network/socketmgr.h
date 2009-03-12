#ifndef SOCKET_MANAGER_H_
#define SOCKET_MANAGER_H_

#include "singleton.h"
#include "socketdefs.h"
#include "mutex.h"
#include "autolock.h"
//#include "smartptr.h"

//using namespace idllib;

namespace srdgame
{
#define SOCKET_HOLDER_SIZE 65536    // You don't want this number to be too big, otherwise you're gonna be eating
                                    // memory. 65536 = 256KB, so thats no big issue for now, and I really can't
                                    // see anyone wanting to have more than 65536 concurrent connections.


// No need to have lock to this class, since all the sockets are started by the worker and will delete and used by the worker only... So lock is useless.  

class Socket;
class ListenSocket;
class SocketWorker;
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
		return _count;
	}

	//  Close all the sockets.
	void close_all();

	void show_info();

	// Should call this to start the worker thread, or all the sockets are not working.
	void start_worker();

	int get_epoll_fd() 
	{
		return _epoll_fd;
	}
protected:	
	size_t _count;
	
    	int _epoll_fd;

    	Socket * _fds[SOCKET_HOLDER_SIZE];
	ListenSocket * _listen_fds[SOCKET_HOLDER_SIZE];
	//SmartPtr<Socket> _fds[SOCKET_HOLDER_SIZE];
	//SmartPtr<ListenSocket> _listen_fds[SOCKET_HOLDER_SIZE];

	SOCKET _max_fd;
};
//SocketMgr& pMgr = SocketMgr::get_singleton();
}
#endif
