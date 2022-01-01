# External Thirdparty script.


set(JuliaSDK_ARCH ${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE})
if(NOT JuliaSDK_ARCH)
    if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
        set(JuliaSDK_ARCH x64)
    else()
        set(JuliaSDK_ARCH x86)
    endif()
endif()

set(JuliaSDK_VERSION 1.7.1)
set(JuliaSDK_ROOT ${THIRDPARTY_DIR}/Julia/${JuliaSDK_VERSION}/${CMAKE_SYSTEM_NAME}/${JuliaSDK_ARCH})

message(STATUS "Loaded JuliaSDK: ${JuliaSDK_VERSION}")
add_library(JuliaSDK INTERFACE)
target_include_directories(JuliaSDK INTERFACE ${JuliaSDK_ROOT}/include/julia)
target_link_directories(JuliaSDK INTERFACE ${JuliaSDK_ROOT}/lib)

add_library(JuliaSDK::libjulia STATIC IMPORTED GLOBAL)
if (WIN32)
    set_target_properties(
        JuliaSDK::libjulia PROPERTIES
        IMPORTED_LOCATION ${JuliaSDK_ROOT}/lib/libjulia.dll.a
    )
endif()

if (MSVC)
    target_link_options(JuliaSDK::libjulia INTERFACE "/DELAYLOAD:libjulia.dll")
endif()

add_library(JuliaSDK::openlibm STATIC IMPORTED GLOBAL)
if (WIN32)
    set_target_properties(
        JuliaSDK::openlibm PROPERTIES
        IMPORTED_LOCATION ${JuliaSDK_ROOT}/lib/libopenlibm.dll.a
    )
endif()
if (MSVC)
    target_link_options(JuliaSDK::openlibm INTERFACE "/DELAYLOAD:libopenlibm.dll")
endif()

target_link_libraries(JuliaSDK INTERFACE
    JuliaSDK::libjulia
    JuliaSDK::openlibm
)


library_post_installbuild( Embed-Systems ${JuliaSDK_ROOT}/bin ${PROJECT_SOURCE_DIR}/.Build/Bin )
# library_post_installbuild( Embed-Systems ${JuliaSDK_ROOT}/lib ${PROJECT_SOURCE_DIR}/.Build/Bin/julia/lib )