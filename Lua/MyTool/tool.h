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

const TCHAR* STR_NULL = _T("");
const TCHAR* STR_LOOP = _T("loop");

extern "C" DLL_SAMPLE_API void log_init();
extern "C" DLL_SAMPLE_API void log_close();

extern "C" DLL_SAMPLE_API void log_debug(const TCHAR*str);
extern "C" DLL_SAMPLE_API void log_warn(const TCHAR*str);
extern "C" DLL_SAMPLE_API void log_error(const TCHAR*str);

extern "C" DLL_SAMPLE_API void log_debug_f(const TCHAR*str, const TCHAR*file = STR_LOOP);
extern "C" DLL_SAMPLE_API void log_info_f(const TCHAR*str, const TCHAR*file = STR_LOOP);
extern "C" DLL_SAMPLE_API void log_warn_f(const TCHAR*str, const TCHAR*file = STR_LOOP);
extern "C" DLL_SAMPLE_API void log_error_f(const TCHAR*str, const TCHAR*file = STR_LOOP);
extern "C" DLL_SAMPLE_API void log_fatal_f(const TCHAR*str, const TCHAR*file = STR_LOOP);


#define LOG_D(STR);				log_debug(STR);
#define LOG_W(STR);				log_warn(STR);
#define LOG_E(STR);				log_error(STR);

#define LOG_D_F(STR,F);			log_debug_f(STR,F);
#define LOG_D_F(STR);			log_debug_f(STR);
#define LOG_I_F(STR,F);			log_info_f(STR,F);
#define LOG_I_F(STR);			log_info_f(STR);
#define LOG_W_F(STR,F);			log_warn_f(STR,F);
#define LOG_W_F(STR);			log_warn_f(STR);
#define LOG_E_F(STR,F);			log_error_f(STR,F);
#define LOG_E_F(STR);			log_error_f(STR);
#define LOG_F_F(STR,F);			log_fatal_f(STR,F);
#define LOG_F_F(STR);			log_fatal_f(STR);


#endif