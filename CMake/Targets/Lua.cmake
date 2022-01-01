
add_library( Scripting-Lua STATIC
    src/Lua/scripting_lua.cpp
    include/Embed-Pipeline/Lua/scripting_lua.h
    scripts/example.lua
)

target_link_libraries( Scripting-Lua PRIVATE Lua Utils )
add_linktoexe( Scripting-Lua )