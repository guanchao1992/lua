#include "tool_log.h"
#include "atlbase.h"
#include "atlstr.h"

extern "C" DLL_SAMPLE_API const char* STR_NULL = _T("");
extern "C" DLL_SAMPLE_API const char* STR_LOOP = _T("loop");

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

static char format_outbuf[5][4096];
static unsigned int outbufnum = 0;
#define DEBUGTYPE	"[-DEBUG-]"
#define WARNTYPE	"[-WARN-]"
#define ERRORTYPE	"[-ERROR-]"
#define INFOTYPE	"[-INFO-]"
#define FATALTYPE	"[-FATAL-]"

#define FORMAT_OUTBUF(typestr); \
	va_list args;\
	int n; \
	++outbufnum; \
	char * outbuf = format_outbuf[outbufnum % 5]; \
	strcpy_s(outbuf,96, typestr);\
	va_start(args, format); \
	n = vsnprintf(outbuf + strlen(typestr), 4000, format, args); \
	va_end(args);


extern "C" DLL_SAMPLE_API void log_debug(const char*format, ...)
{
	FORMAT_OUTBUF(DEBUGTYPE);
	LogLog::getLogLog()->debug(outbuf);
	
}

extern "C" DLL_SAMPLE_API void log_warn(const char*format, ...)
{
	FORMAT_OUTBUF(WARNTYPE);
	LogLog::getLogLog()->warn(outbuf);
}

extern "C" DLL_SAMPLE_API void log_error(const char*format, ...)
{
	FORMAT_OUTBUF(ERRORTYPE);
	LogLog::getLogLog()->error(outbuf);
}

extern "C" DLL_SAMPLE_API void log_debug_f(const char*loop, const char*format, ...)
{
	FORMAT_OUTBUF(DEBUGTYPE);
	NDCContextCreator _context(loop);
	LOG4CPLUS_DEBUG(Logger::getRoot(), outbuf);
	LogLog::getLogLog()->debug(outbuf);
}

extern "C" DLL_SAMPLE_API void log_info_f(const char*loop, const char*format, ...)
{
	FORMAT_OUTBUF(INFOTYPE);
	NDCContextCreator _context(loop);
	LOG4CPLUS_INFO(Logger::getRoot(), outbuf);
	LogLog::getLogLog()->debug(outbuf);
}

extern "C" DLL_SAMPLE_API void log_warn_f(const char*loop, const char*format, ...)
{
	FORMAT_OUTBUF(WARNTYPE);
	NDCContextCreator _context(loop);
	LOG4CPLUS_WARN(Logger::getRoot(), outbuf);
	LogLog::getLogLog()->debug(outbuf);
}

extern "C" DLL_SAMPLE_API void log_error_f(const char*loop, const char*format, ...)
{
	FORMAT_OUTBUF(ERROR);
	NDCContextCreator _context(loop);
	LOG4CPLUS_ERROR(Logger::getRoot(), outbuf);
	LogLog::getLogLog()->debug(outbuf);
}

extern "C" DLL_SAMPLE_API void log_fatal_f(const char*loop, const char*format, ...)
{
	FORMAT_OUTBUF(FATALTYPE);
	NDCContextCreator _context(loop);
	LOG4CPLUS_FATAL(Logger::getRoot(), outbuf);
	LogLog::getLogLog()->debug(outbuf);
}


