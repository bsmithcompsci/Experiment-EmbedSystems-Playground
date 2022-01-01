set(LuaJIT_VERSION 2.0.5)

set(LuaJIT_ARCH ${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE})
if(NOT LuaJIT_ARCH)
    if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
        set(LuaJIT_ARCH x64)
    else()
        set(LuaJIT_ARCH x86)
    endif()
endif()

set(LuaJIT_BUILD_DIR "${THIRDPARTY_DIR}/LuaJIT/${LuaJIT_VERSION}/${CMAKE_SYSTEM_NAME}/${LuaJIT_ARCH}")
set(LuaJIT_ROOT "${THIRDPARTY_DIR}/LuaJIT/${LuaJIT_VERSION}")
message(STATUS "Loaded LuaJIT: ${LuaJIT_VERSION}")

add_library(LuaJIT INTERFACE)
target_include_directories(LuaJIT INTERFACE ${LuaJIT_ROOT}/src)

add_library(LuaJIT::lua51 STATIC IMPORTED GLOBAL)
if (WIN32)
    set_target_properties(
        LuaJIT::lua51 PROPERTIES
        IMPORTED_LOCATION ${LuaJIT_BUILD_DIR}/lib/lua51.lib
    )
endif()
add_library(LuaJIT::luajit STATIC IMPORTED GLOBAL)
if (WIN32)
    set_target_properties(
        LuaJIT::luajit PROPERTIES
        IMPORTED_LOCATION ${LuaJIT_BUILD_DIR}/lib/luajit.lib
    )
endif()

target_link_libraries(LuaJIT INTERFACE
    LuaJIT::lua51
    LuaJIT::luajit
)
library_post_installbuild( Embed-Systems ${LuaJIT_BUILD_DIR}/bin ${PROJECT_SOURCE_DIR}/.Build/Bin )

