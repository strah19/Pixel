#include "pixelpch.h"
#include "Scripting.h"
#include "Core/Logger.h"

namespace Pixel {
	void RunTestScript() {
		std::string command = "";
		lua_State* L = luaL_newstate();
		luaL_openlibs(L);

		int r = luaL_dofile(L, "script_test.lua");

		if (CheckScriptError(L, r)) {
			lua_getglobal(L, "a");
			if (lua_isnumber(L, -1)) {
				float a = (float)lua_tonumber(L, -1);
				std::cout << a << std::endl;
			}
		}

		lua_close(L);
	}

	bool CheckScriptError(lua_State* L, int result) {
		if (result != LUA_OK) {
			PIXEL_LOG_ERROR("%s", lua_tostring(L, -1));
			return false;
		}
		return true;
	}
}