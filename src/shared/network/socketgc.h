#ifndef SOCKET_GC_H_
#define SOCKET_GC_H_

#include "singleton.h"
#include "threadbase.h"
#include "fastqueue.h"

namespace srdgame
{
class Socket;
class SocketGC : public Singleton < SocketGC >
{
public:
	SocketGC();
	~SocketGC();
	void GC(Socket* s);

private:
	class GCWorker : public ThreadBase
	{
	public:
		GCWorker() : _closing (false)
		{
		}
		// methods from ThreadBase
		//
		// blocking function, return true means delete this threadobject.
		virtual bool run();
		// call to shutdown this thread.
		virtual void shutdown(){ _closing = true;}
		// call when this get closing? not work yet?
		virtual void on_close(){}
		// call to checking whether it is running.
		virtual bool is_running(){return !_closing;}
	private:
		bool _closing;
	};
	struct GCData
	{
		Socket* _socket;
		int _time;
	};
private:
	Mutex _worker_lock;
	GCWorker* _worker;
	FastQueue<GCData> _data;
	bool _closing;
};
}

#endif
