
add_library( Scripting-CSharp STATIC
    src/CSharp/scripting_csharp.cpp
    include/Embed-Pipeline/CSharp/scripting_csharp.h
    scripts/example.cs/MainClass.cs
)

target_link_libraries( Scripting-CSharp PRIVATE Mono Utils )
add_linktoexe( Scripting-CSharp )

library_post_installbuild( Embed-Systems ${THIRDPARTY_DIR}/Mono/lib/mono/4.5 ${PROJECT_SOURCE_DIR}/.Build/Bin/Mono/4.5 )