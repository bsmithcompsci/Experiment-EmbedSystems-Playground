#include "../../include/Embed-Pipeline/Lua/scripting_lua.h"
#include <string>
#include <iostream>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
};

namespace Scripting
{
	// We don't want to share this with other includes, so... we are gonna hide it in the cpp.
	static lua_State *state = nullptr;

	LuaPipeline::LuaPipeline()
	{

		// Initialize lua state.
		state = luaL_newstate();

		//lua_setallocf(state, customAllocationFunc, nullptr);

		// open all the libraries that lua comes with.
		luaL_openlibs(state);
	}
	LuaPipeline::~LuaPipeline()
	{
		lua_close(state);
	}
	bool LuaPipeline::ExecFile(const char *_filePath)
	{
		int r = luaL_dofile(state, _filePath);
		if (r != LUA_OK)
		{
			std::string errorMsg = lua_tostring(state, -1);
			std::cerr << "Scripting Error[Lua] ::> " << errorMsg << "\n";
		}
		return r == LUA_OK;
	}
	int LuaPipeline::Exec(const char *_cmdline)
	{
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
