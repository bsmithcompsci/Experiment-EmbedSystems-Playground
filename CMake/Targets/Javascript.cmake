
add_library( Scripting-Javascript STATIC
    src/Javascript/scripting_javascript.cpp
    include/Embed-Pipeline/Javascript/scripting_javascript.h
    scripts/example.js
)

# https://stackoverflow.com/questions/62921373/embedder-side-pointer-compression-is-disabled
if (MSVC)
    target_compile_definitions(Scripting-Javascript PUBLIC V8_COMPRESS_POINTERS V8_31BIT_SMIS_ON_64BIT_ARCH)
endif()

target_link_libraries( Scripting-Javascript PRIVATE V8::v8 V8::libplatform Utils )
add_linktoexe( Scripting-Javascript )