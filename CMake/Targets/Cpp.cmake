add_library( Scripting-Cpp STATIC
    src/Cpp/scripting_cpp.cpp
    include/Embed-Pipeline/Cpp/scripting_cpp.h
    scripts/example.hpp
)
target_include_directories(Scripting-Cpp PRIVATE scripts)
target_link_libraries( Scripting-Cpp PRIVATE Utils )
add_linktoexe( Scripting-Cpp )
