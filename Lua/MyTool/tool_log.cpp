#include "tool_log.h"
#include "atlbase.h"
#include "atlstr.h"

extern "C" DLL_SAMPLE_API const TCHAR* STR_NULL = _T("");
extern "C" DLL_SAMPLE_API const TCHAR* STR_LOOP = _T("loop");

extern "C" DLL_SAMPLE_API void log_init()
{
	log4cplus::initialize();
	helpers::LogLog::getLogLog()->setInternalDebugging(true);
	SharedFileAppenderPtr append(
		new DailyRollingFileAppender(LOG4CPLUS_TEXT("log/log"),
			LOG4CPLUS_TEXT(".log"), MINUTELY, true, 10, true));
	append->setName(LOG4CPLUS_TEXT("First"));
	append->setLayout(std::auto_ptr<Layout>(new TTCCLayout()));
	append->getloc();
	Logger::getRoot().addAppender(SharedAppenderPtr(append.get()));
}

extern "C" DLL_SAMPLE_API void log_close()
{
	log4cplus::Logger::shutdown();
}

extern "C" DLL_SAMPLE_API void log_debug(const TCHAR*str)
{
	LogLog::getLogLog()->debug(LOG4CPLUS_C_STR_TO_TSTRING(str));
	OutputDebugString(str);
}

extern "C" DLL_SAMPLE_API void log_warn(const TCHAR*str)
{
	LogLog::getLogLog()->warn(LOG4CPLUS_C_STR_TO_TSTRING(str));
	OutputDebugString(str);
}

extern "C" DLL_SAMPLE_API void log_error(const TCHAR*str)
{
	LogLog::getLogLog()->error(LOG4CPLUS_C_STR_TO_TSTRING(str));
	OutputDebugString(str);
}

extern "C" DLL_SAMPLE_API void log_debug_f(const TCHAR*str, const TCHAR*file)
{
	NDCContextCreator _context(file);
	LOG4CPLUS_DEBUG(Logger::getRoot(), str);
}

extern "C" DLL_SAMPLE_API void log_info_f(const TCHAR*str, const TCHAR*file)
{
	NDCContextCreator _context(file);
	LOG4CPLUS_INFO(Logger::getRoot(), str);
}

extern "C" DLL_SAMPLE_API void log_warn_f(const TCHAR*str, const TCHAR*file)
{
	NDCContextCreator _context(file);
	LOG4CPLUS_WARN(Logger::getRoot(), str);
}

extern "C" DLL_SAMPLE_API void log_error_f(const TCHAR*str, const TCHAR*file)
{
	NDCContextCreator _context(file);
	LOG4CPLUS_ERROR(Logger::getRoot(), str);
}

extern "C" DLL_SAMPLE_API void log_fatal_f(const TCHAR*str, const TCHAR*file)
{
	NDCContextCreator _context(file);
	LOG4CPLUS_FATAL(Logger::getRoot(), str);
}
