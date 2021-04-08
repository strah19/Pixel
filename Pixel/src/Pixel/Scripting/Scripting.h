#ifndef SCRIPTING_H
#define SCRIPTING_H

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace Pixel {
	void RunTestScript();
	bool CheckScriptError(lua_State* L, int result);
}

#endif // !SCRIPTING_H
