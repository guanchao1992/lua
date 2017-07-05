#ifndef __CToLua_H__
#define __CToLua_H__

#ifdef DLL_SAMPLE_EXPORTS
#define DLL_SAMPLE_API __declspec(dllexport)
#else
#define DLL_SAMPLE_API __declspec(dllimport)
#endif

extern "C" DLL_SAMPLE_API int _stdcall CTOLuaAdd(int a,int b);

#endif // !__CToLua_H__
