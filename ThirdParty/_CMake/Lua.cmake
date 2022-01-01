set(Lua_VERSION 5.4.3)
set(Lua_DIR "${THIRDPARTY_DIR}/Lua/${Lua_VERSION}")
message(STATUS "Loaded LuaJIT: ${Lua_VERSION}")
file(GLOB Scripting_Lua_Engine_Source "${Lua_DIR}/src/*.*")

add_library(
    Lua STATIC
    ${Scripting_Lua_Engine_Source}
)
target_include_directories( Lua PUBLIC "${Lua_DIR}/src/" )
set_target_properties( Lua PROPERTIES FOLDER "Thirdparties")