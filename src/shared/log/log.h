#ifndef LOG_H
#define LOG_H

#include "singleton.h"
#include <cstdarg>
//------------------------------------------------------------------------------
namespace srdgame
{
class LogFile;
enum LogLevel
{
	LL_NONE = 0,
	LL_SUCCESS,
	LL_ERROR,
	LL_WARNING,
	LL_NOTICE,
	LL_DEBUG,
};
class Log : public Singleton<Log>
{
public:
	void setup(LogLevel s_lvl, LogLevel f_lvl)
	{
		_s_lvl = s_lvl;
		_f_lvl = f_lvl;
	}
	void bind(LogFile* file)
	{
		_file = file;
	}
	
	void append(LogLevel lvl, const char* source, const char* format, ...);

	void out_line();
private:
	LogFile* _file;
	LogLevel _s_lvl;
	LogLevel _f_lvl;
};
Log& LOG = Log::get_singleton();
inline void LogSuccess(const char* source, const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	LOG.append(LL_SUCCESS, source, format, ap);
	va_end(ap);
}

inline void LogError(const char* source, const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	LOG.append(LL_ERROR, source, format, ap);
	va_end(ap);
}
inline void LogWarning(const char* source, const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	LOG.append(LL_WARNING, source, format, ap);
	va_end(ap);
}

inline void LogNotice(const char* source, const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	LOG.append(LL_NOTICE, source, format, ap);
	va_end(ap);
}
inline void LogDebug(const char* source, const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	LOG.append(LL_DEBUG, source, format, ap);
	va_end(ap);
}
inline void LogLine()
{
	LOG.out_line();
}
}
#endif
//------------------------------------------------------------------------------
