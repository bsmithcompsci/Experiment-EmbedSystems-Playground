# External Thirdparty script.

set(OpenJDK_ARCH ${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE})
if(NOT OpenJDK_ARCH)
    if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
        set(OpenJDK_ARCH x64)
    else()
        set(OpenJDK_ARCH x86)
    endif()
endif()

set(OpenJDK_VERSION 17.0.1)
set(OpenJDK_ROOT ${THIRDPARTY_DIR}/OpenJDK/${OpenJDK_VERSION}/${CMAKE_SYSTEM_NAME}/${OpenJDK_ARCH})

message(STATUS "Loaded OpenJDK: ${OpenJDK_VERSION}")
if (NOT EXISTS "${OpenJDK_ROOT}")
    message(FATAL_ERROR "OpenJDK is not support on your system, visit: https://www.openlogic.com/openjdk-downloads")
endif()

add_library(OpenJDK INTERFACE)
target_include_directories(OpenJDK INTERFACE ${OpenJDK_ROOT}/include)
if (WIN32)
    target_include_directories(OpenJDK INTERFACE ${OpenJDK_ROOT}/include/win32)
endif()
target_link_directories(OpenJDK INTERFACE ${OpenJDK_ROOT}/lib)

add_library(OpenJDK::jvm STATIC IMPORTED GLOBAL)
if (WIN32)
    set_target_properties(
        OpenJDK::jvm PROPERTIES
        IMPORTED_LOCATION ${OpenJDK_ROOT}/lib/jvm.lib
    )
endif()

if (MSVC)
    target_link_options(OpenJDK::jvm INTERFACE "/DELAYLOAD:jvm.dll")
endif()

target_link_libraries(OpenJDK INTERFACE
    OpenJDK::jvm
)

library_post_installbuild( Embed-Systems ${OpenJDK_ROOT}/bin ${PROJECT_SOURCE_DIR}/.Build/Bin/jre/bin )
library_post_installbuild( Embed-Systems ${OpenJDK_ROOT}/lib ${PROJECT_SOURCE_DIR}/.Build/Bin/jre/lib )