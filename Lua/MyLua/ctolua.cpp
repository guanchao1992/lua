#include "ctolua.h"

#include <string.h>


extern "C" DLL_SAMPLE_API int _stdcall CTOLuaAdd(int a, int b)
{
	return a + b;
}