#pragma once
#include "..\MyLua\ctolua.h"
#include "..\MyTool\tool_log.h"
#include "base/lua_get.h"

typedef std::map<const char*, lua_CFunction> MapClassFunc;

void regAllClass();
