add_library( Scripting-Java STATIC
    src/Java/scripting_java.cpp
    include/Embed-Pipeline/Java/scripting_java.h
)

target_link_libraries( Scripting-Java PRIVATE OpenJDK Utils )
add_linktoexe( Scripting-Java )
