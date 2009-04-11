#include "updater.h"
#include "threadbase.h"
#include "fastqueue.h"
#include <map>

namespace srdgame
{
class WorldUpdater : public Updater
{
	class Thread : public ThreadBase
	{
	public:
		Thread(WorldUpdater* owner);
		virtual ~Thread();
		// blocking function, return true means this thread finished to run, 
		// return false means continue to call this run function
		virtual bool run();
		// call to shutdown this thread.
		virtual void shutdown();
		// call when this get closing? not work yet?
		virtual void on_close();
		// call to checking whether it is running.
		virtual bool is_running();
	private:
		bool _closing;
		WorldUpdater* _owner;
	};
public:
	WorldUpdater();
	virtual ~WorldUpdater();

	virtual void start();
	virtual void pause();
	virtual void stop();

	virtual void add(NObject* obj);
	virtual void remove(NObject* obj);

private:
	void update(); // called by thread,
	void select();
private:
	struct PQueue
	{
		PQueue() : _delay(0){}
		int _delay;
		FastQueue<NObject*>* _queue;
	};
	PQueue _queues[NP_COUNT];
	
	int _doing;
	FastQueue<NObject*> * _new_queue;
	
	std::map<NObject*, bool> _removing;
	Thread* _thread;
	bool _pause;
	Mutex _lock;
};
}
