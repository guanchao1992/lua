#include "tool.h"

static Logger* s_logger;

inline Logger getLOG()
{
	return *s_logger;
}

extern "C" DLL_SAMPLE_API void initLog()
{
	s_logger = new Logger();
	*s_logger = Logger::getInstance(LOG4CPLUS_TEXT("log"));
	log4cplus::initialize();
}

extern "C" DLL_SAMPLE_API void log_debug(const char*str)
{
	LogLog::getLogLog()->debug(LOG4CPLUS_C_STR_TO_TSTRING(str));
}

extern "C" DLL_SAMPLE_API void log_warn(const char*str)
{
	LogLog::getLogLog()->warn(LOG4CPLUS_C_STR_TO_TSTRING(str));
}

extern "C" DLL_SAMPLE_API void log_error(const char*str)
{
	LogLog::getLogLog()->error(LOG4CPLUS_C_STR_TO_TSTRING(str));
}

extern "C" DLL_SAMPLE_API void log_trace_f(const char*str)
{
	LOG4CPLUS_TRACE_METHOD(getLOG(), LOG4CPLUS_C_STR_TO_TSTRING(str));
}

extern "C" DLL_SAMPLE_API void log_debug_f(const char*str)
{
	LOG4CPLUS_DEBUG(getLOG(), LOG4CPLUS_C_STR_TO_TSTRING(str));
}

extern "C" DLL_SAMPLE_API void log_info_f(const char*str)
{
	LOG4CPLUS_INFO(getLOG(), LOG4CPLUS_C_STR_TO_TSTRING(str));
}

extern "C" DLL_SAMPLE_API void log_warn_f(const char*str)
{
	LOG4CPLUS_WARN(getLOG(), LOG4CPLUS_C_STR_TO_TSTRING(str));
}

extern "C" DLL_SAMPLE_API void log_error_f(const char*str)
{
	LOG4CPLUS_ERROR(getLOG(), LOG4CPLUS_C_STR_TO_TSTRING(str));
}

extern "C" DLL_SAMPLE_API void log_fatal_f(const char*str)
{
	LOG4CPLUS_FATAL(getLOG(), LOG4CPLUS_C_STR_TO_TSTRING(str));
}
