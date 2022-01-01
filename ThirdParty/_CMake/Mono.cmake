# External Thirdparty script.

message(STATUS "Loaded Mono: 2.0")
add_library(Mono INTERFACE)
target_include_directories(Mono INTERFACE ${THIRDPARTY_DIR}/Mono/include/mono-2.0)

set(MONO_STATICLY_LINKED ON)

if (MONO_STATICLY_LINKED)
    add_library(Mono::_static_ STATIC IMPORTED GLOBAL)
    if (WIN32)
        set_target_properties(
            Mono::_static_ PROPERTIES
            IMPORTED_LOCATION ${THIRDPARTY_DIR}/Mono/lib/libmono-static-sgen.lib
        )
    endif()
    target_link_libraries(Mono INTERFACE
        Mono::_static_

        # Windows required libraries.
        Winmm.lib
        Ws2_32.lib
        crypt32.lib
        bcrypt.lib
        version.lib
    )
endif(MONO_STATICLY_LINKED)