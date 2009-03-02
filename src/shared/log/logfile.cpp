#include "logfile.h"

using namespace std;
using namespace srdgame;

LogFile::LogFile(const char* fn)
{
	_file_lock.lock();
	_file.open(fn, ios::app | ios::out);
	_file_lock.unlock();
}
LogFile::~LogFile()
{
	shutdown();
	_file_lock.lock();
	_file.close();
	_file_lock.unlock();
	_logs_lock.lock();
	while (!_logs.empty())
	{
		_logs.pop();
	}
	_logs_lock.unlock();
}
void LogFile::append(const char* source, const char* format, ...)
{
	// TODO: Anyway to do with this? I do not have to have 4096 buffer, really!!!
	char buf[4096];
	memset(buf, 0, 4096);
	va_list ap;
	va_start(ap, format);
	sprint(buf, format, ap);
	_logs_lock.lock();
	_logs.push(string(buf));
	_logs_lock.unlock();
	va_end(ap);

}

bool LogFile::run()
{
	while (_running)
	{
		_logs_lock.lock();
		if (_logs_lock.empty())
		{
			_logs_lock.unlock();
			sleep(20);
			continue;
		}
		_file_lock.lock();

		if (!_file.good())
			break;

		while (!_logs.empty())
		{
			_file << _logs.front();
			_file << "\n";
			_logs.pop();
		}
		_file_lock.unlock();
		_logs_lock.unlock();
		sleep(20);

	}
	// return true to delete this thread, because we no longer need this thread?
	return true;
}

void LogFile::on_close()
{
	// TODO: anything to do?
}
