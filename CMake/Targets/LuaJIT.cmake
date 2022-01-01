add_library( Scripting-LuaJIT STATIC
    src/LuaJIT/scripting_luajit.cpp
    include/Embed-Pipeline/LuaJIT/scripting_luajit.h
    scripts/example.lua
)

target_link_libraries( Scripting-LuaJIT PRIVATE LuaJIT Utils )
add_linktoexe( Scripting-LuaJIT )