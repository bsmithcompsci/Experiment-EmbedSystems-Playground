#include "../../include/Embed-Pipeline/LuaJIT/scripting_luajit.h"
#include <string>
#include <iostream>

#include <lua.hpp>
#define LUA_OK 0

namespace Scripting
{
	// We don't want to share this with other includes, so... we are gonna hide it in the cpp.
	static lua_State *state = nullptr;

	LuaJITPipeline::LuaJITPipeline()
	{
		// Initialize lua state.
		state = luaL_newstate();
		if (state == nullptr)
		{
			return;
		}

		//lua_setallocf(state, customAllocationFunc, nullptr);

		// open all the libraries that lua comes with.
		luaL_openlibs(state);
	}
	LuaJITPipeline::~LuaJITPipeline()
	{
		lua_close(state);
	}
	bool LuaJITPipeline::ExecFile(const char *_filePath)
	{
		if (state == nullptr)
			return false;

		int r = luaL_dofile(state, _filePath);
		if (r != LUA_OK)
		{
			std::string errorMsg = lua_tostring(state, -1);
			std::cerr << "Scripting Error[Lua] ::> " << errorMsg << "\n";
		}
		return r == LUA_OK;
	}
	int LuaJITPipeline::Exec(const char *_cmdline)
	{
		if (state == nullptr)
			return -1;

		int r = luaL_dostring(state, _cmdline);
		if (r != LUA_OK)
		{
			std::string errorMsg = lua_tostring(state, -1);
			std::cerr << "Scripting Error[Lua] ::> " << errorMsg << "\n";
			return -1;
		}
		if (!lua_isnumber(state, -1))
		{
			std::cerr << "Scripting Error[Lua] ::> Expected a `Number` instead got: " << lua_typename(state, -1) << "\n";
			return -1;
		}
		int val = (int)lua_tonumber(state, -1);
		std::cout << val << "\n";
		return val;
	}
};
