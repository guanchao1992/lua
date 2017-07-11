#ifndef __TOOL_H__
#define __TOOL_H__

#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/helpers/fileinfo.h>
#include <log4cplus/loggingmacros.h>

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

#ifdef DLL_TOOL_EXPORTS
#define DLL_SAMPLE_API __declspec(dllexport)
#else
#define DLL_SAMPLE_API __declspec(dllimport)
#endif


extern "C" DLL_SAMPLE_API void initLog();

extern "C" DLL_SAMPLE_API void log_debug(const char*str);
extern "C" DLL_SAMPLE_API void log_warn(const char*str);
extern "C" DLL_SAMPLE_API void log_error(const char*str);


extern "C" DLL_SAMPLE_API void log_trace_f(const char*str);
extern "C" DLL_SAMPLE_API void log_debug_f(const char*str);
extern "C" DLL_SAMPLE_API void log_info_f(const char*str);
extern "C" DLL_SAMPLE_API void log_warn_f(const char*str);
extern "C" DLL_SAMPLE_API void log_error_f(const char*str);
extern "C" DLL_SAMPLE_API void log_fatal_f(const char*str);


#define LOG_D(STR);				log_debug(STR);
#define LOG_W(STR);				log_warn(STR);
#define LOG_E(STR);				log_error(STR);

#define LOG_F_T(STR);			log_trace_f(STR);
#define LOG_F_D(STR);			log_debug_f(STR);
#define LOG_F_I(STR);			log_info_f(STR);
#define LOG_F_W(STR);			log_warn_f(STR);
#define LOG_F_E(STR);			log_error_f(STR);
#define LOG_F_F(STR);			log_fatal_f(STR);



#endif