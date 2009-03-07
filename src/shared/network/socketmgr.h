#ifndef SOCKET_MANAGER_H_
#define SOCKET_MANAGER_H_

#include "singleton.h"
#include "socketdefs.h"
#include "mutex.h"

namespace srdgame
{
#define SOCKET_HOLDER_SIZE 30000    // You don't want this number to be too big, otherwise you're gonna be eating
                                    // memory. 65536 = 256KB, so thats no big issue for now, and I really can't
                                    // see anyone wanting to have more than 65536 concurrent connections.

#define SOCKET_LISTEN_HOLDER_SIZE 32 // should be much more than you need.

class Socket;
class ListenSocket;
class SocketManager : public Singleton<SocketManager>
{
	friend class SocketWorker;
public:
	SocketManager();
	~SocketManager();
public:
	void add(Socket* s);
	void add(ListenSocket* s);
	void remove(Socket* s);
	inline size_t count()
	{
		return _count;
	}
	void close_all();

	void show_info();

	void start_worker();
protected:	
	size_t _count;
	
    	int _epoll_fd;

    	Socket * _fds[SOCKET_HOLDER_SIZE];
	ListenSocket * _listen_fds[SOCKET_LISTEN_HOLDER_SIZE];

	SOCKET _max_fd;

	static Mutex _lock;
};
//SocketManager& pManager = SocketManager::get_singleton();
}
#endif
