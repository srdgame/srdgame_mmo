#include "log.h"
#include "loggui.h"
#include "logfile.h"
using namespace srdgame;

#define GUI LogGui::get_singleton()

//------------------------------------------------------------------------------
void Log::append(LogLevel lvl, const char* source, const char* format, ...)
{
//	printf("************************");
	va_list ap;
	va_start(ap, format);

	LogLevel s_lvl = lvl <= _s_lvl ? lvl : LL_NONE;
	switch (s_lvl)
	{
		case LL_SUCCESS:
			GUI.Success(source, format, ap);
			break;
		case LL_ERROR:
			GUI.Error(source, format, ap);
			break;
		case LL_WARNING:
			GUI.Warning(source, format, ap);
			break;
		case LL_NOTICE:
			GUI.Notice(source, format, ap);
			break;
		case LL_DEBUG:
			GUI.Debug(source, format, ap);
			break;
		default:
			//GUI.Debug(source, format, ap);
			break;
	}
	if (_file && lvl <= _f_lvl)
	{
		_file->append(source, format, ap);
	}
	va_end(ap);
}
//------------------------------------------------------------------------------
void Log::append(LogLevel lvl, const char* source, const char* format, va_list ap)
{
	// vprintf(format, ap);
//	printf("===========================");
	LogLevel s_lvl = lvl <= _s_lvl ? lvl : LL_NONE;
	switch (s_lvl)
	{
		case LL_SUCCESS:
			GUI.Success(source, format, ap);
			break;
		case LL_ERROR:
			GUI.Error(source, format, ap);
			break;
		case LL_WARNING:
			GUI.Warning(source, format, ap);
			break;
		case LL_NOTICE:
			GUI.Notice(source, format, ap);
			break;
		case LL_DEBUG:
			GUI.Debug(source, format, ap);
			break;
		default:
			//GUI.Debug(source, format, ap);
			break;
	}
	if (_file && lvl <= _f_lvl)
	{
		_file->append(source, format, ap);
	}
}
void Log::out_line()
{
	GUI.Line();
	if (_file)
	{
		_file->append(NULL, "====================================================================================");
	}
}


