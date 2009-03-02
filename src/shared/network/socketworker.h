#ifndef SOCKET_WORKER_H_
#define SOCKET_WORKER_H_

#include "socketdefs.h"
#include "threadbase.h"

#define THREAD_EVENT_SIZE 4096      // This is the number of socket events each thread can receieve at once.
                                    // This default value should be more than enough.

namespace srdgame
{
// the socket thread.
class SocketWorker : public ThreadBase
{
public:
	SocketWorker();
	virtual ~SocketWorker();
	virtual bool run();
	inline virtual void shutdown(){_running = false;}
	virtual void on_close();
	inline virtual bool is_running()
	{
		return _running;
	}
protected:
	bool _running;
	struct epoll_event _events[THREAD_EVENT_SIZE];
};
}

#endif
