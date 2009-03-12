
// Any data coming from socket, will spawn one worker for it.
// But how to make sure only spawn one?  Using RealManager???
//

#ifndef LOGIN_WORKER_H_
#define LOGIN_WORKER_H_

#include "threadbase.h"

namespace srdgame
{
class WorldSocketBase;
struct Packet;
class WorldWorker : public ThreadBase
{
public:
	WorldWorker(WorldSocketBase* socket);
	virtual ~WorldWorker();
public:
	// methods from ThreadBase
	//
	// blocking function, return true means delete this threadobject.
	virtual bool run();
	// call to shutdown this thread.
	virtual void shutdown();
	// call when this get closing? not work yet?
	virtual void on_close();
	// call to checking whether it is running.
	virtual bool is_running();

protected:
	void handle(Packet* packet);
	bool send(Packet* packet);

protected:
	bool _running;
	WorldSocketBase* _socket;
};
}

#endif
