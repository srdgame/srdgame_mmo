#ifndef LOG_FILE_H_
#define LOG_FILE_H_

#include "mutex.h"
#include "threadbase.h"
#include <fstream>
#include <queue>

namespace srdgame
{
class LogFile : public ThreadBase
{
public:
	LogFile(const char* fn);
	virtual ~LogFile();

	virtual void append(const char* source, const char* format, ...);

public:
	// Thread stuff.
	//
	// blocking function, return true means delete this threadobject.
	virtual bool run();
	// call to shutdown this thread.
	virtual void shutdown()
	{
		_running = false;
	}
	// call when this get closing? not work yet?
	virtual void on_close();
	// call to checking whether it is running.
	virtual bool is_running()
	{
		return _running;
	}
protected:
	std::fstream _file;
	Mutex _file_lock;

	std::queue<std::string> _logs;
	Mutex _logs_lock;

	bool _running;
};
}
#endif
