#include "logfile.h"
#include "loggui.h"
#include <cstring>
#include <cstdarg>

using namespace std;
using namespace srdgame;

LogFile::LogFile(const char* fn) : _running(false), _fn(fn)
{
}
LogFile::~LogFile()
{
	shutdown();
}
void LogFile::append(const char* source, const char* format, ...)
{
	// TODO: Anyway to do with this? I do not have to have 4096 buffer, really!!!
	char buf[4096];
	memset(buf, 0, 4096);
	va_list ap;
	va_start(ap, format);
	sprintf(buf, format, ap);
	va_end(ap);

	std::string str(buf);
	_logs.push(str);
}

bool LogFile::run()
{
	_file.open(_fn.c_str(), ios::app | ios::out);
	if (!_file.good())
	{
		LogGui::get_singleton().Error("LogFile", "Could not open log file: %s", _fn.c_str());
		return true;
	}

	while (_running)
	{
		std::string log;
		if (!_logs.try_pop(log))
		{
			usleep(2000);
			continue;
		}

		if (!_file.good())
		{
			LogGui::get_singleton().Error("LogFile", "Could not write to log file: %s", _fn.c_str());
			break;
		}

		_file << log << "\n";

	}
	// return true to delete this thread, because we no longer need this thread?
	return true;
}

void LogFile::on_close()
{
	// TODO: anything to do?
}
