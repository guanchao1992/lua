#ifndef __TOOL_H__
#define __TOOL_H__

#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/ndc.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/property.h>
#include <log4cplus/loggingmacros.h>

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

#ifdef DLL_TOOL_EXPORTS
#define DLL_SAMPLE_API __declspec(dllexport)
#else
#define DLL_SAMPLE_API __declspec(dllimport)
#endif

extern "C" DLL_SAMPLE_API const char* STR_NULL;
extern "C" DLL_SAMPLE_API const char* STR_LOOP;

extern "C" DLL_SAMPLE_API void log_init();
extern "C" DLL_SAMPLE_API void log_close();

extern "C" DLL_SAMPLE_API void log_debug(const char*format, ...);
extern "C" DLL_SAMPLE_API void log_warn(const char*format, ...);
extern "C" DLL_SAMPLE_API void log_error(const char*format, ...);

extern "C" DLL_SAMPLE_API void log_debug_f(const char*loop, const char*format, ...);
extern "C" DLL_SAMPLE_API void log_info_f(const char*loop, const char*format, ...);
extern "C" DLL_SAMPLE_API void log_warn_f(const char*loop, const char*format, ...);
extern "C" DLL_SAMPLE_API void log_error_f(const char*loop, const char*format, ...);
extern "C" DLL_SAMPLE_API void log_fatal_f(const char*loop, const char*format, ...);


#define LOG_D(FORMAT,...);				log_debug(FORMAT,__VA_ARGS__);
#define LOG_W(FORMAT,...);				log_warn(FORMAT,__VA_ARGS__);
#define LOG_E(FORMAT,...);				log_error(FORMAT,__VA_ARGS__);

#define LOG_D_F(LOOP,FORMAT,...); 		log_debug_f(LOOP,FORMAT,__VA_ARGS__);
#define LOG_I_F(LOOP,FORMAT,...); 		log_info_f(LOOP,FORMAT,__VA_ARGS__);
#define LOG_W_F(LOOP,FORMAT,...); 		log_warn_f(LOOP,FORMAT,__VA_ARGS__);
#define LOG_E_F(LOOP,FORMAT,...); 		log_error_f(LOOP,FORMAT,__VA_ARGS__);
#define LOG_F_F(LOOP,FORMAT,...); 		log_fatal_f(LOOP,FORMAT,__VA_ARGS__);


#endif