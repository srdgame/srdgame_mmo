#ifndef G_MSG_H_
#define G_MSG_H_

#include "singleton.h"
#include "fastqueue.h"
#include "nobject.h"

#include <string>

// This is the globle message sender.

namespace srdgame
{
class Player;
class GMsg : public Singleton < GMsg >, public NObject
{
public:
	GMsg();
	~GMsg();
	void send(const char* msg);
	void send(int mid, const char* msg);
	void send(const char* msg, const Player* player);
	void send(int mid, const char* msg, const Player* player);

	virtual long notify(long time);
	virtual NPriority get_priority() {return NP_HAVE_TO;};
	virtual Position* get_pos()	{return NULL;}; 
private:
	struct MSG
	{
		inline MSG() : _mid(-1), _pid(0)
		{
		}
		inline MSG(int mid, const char* msg, int pid) : _mid (mid), _pid(pid)
		{
			_msg = new std::string(msg);
		}
		int _mid; // -1 will send to all the mids.
		std::string* _msg; // message.
		int _pid; // from char_id. 0 is system.
	};
	FastQueue<MSG> _msgs;
};
}

#endif
