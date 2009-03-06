#ifndef LOG_H_
#define LOG_H_

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
	Log() : _file(0), _s_lvl(LL_DEBUG), _f_lvl(LL_DEBUG)
	{
	}
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
	void append(LogLevel lvl, const char* source, const char* format, va_list ap);

	void out_line();
private:
	LogFile* _file;
	LogLevel _s_lvl;
	LogLevel _f_lvl;
};
#define LOG Log::get_singleton()
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
