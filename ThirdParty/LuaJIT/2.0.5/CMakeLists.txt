# Custom Script added to LuaJIT because... there build.bat files don't fit this project's setup process...

cmake_minimum_required(VERSION 3.17)
project(LuaJIT LANGUAGES C VERSION 2.0.5)

# set(ARCH ${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE})
# if(NOT ARCH)
#     if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
#         set(ARCH x64)
#     else()
#         set(ARCH x86)
#     endif()
# endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RAW "${CMAKE_CURRENT_SOURCE_DIR}/.Build/${CMAKE_SYSTEM_NAME}/${ARCH}/Bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RAW "${CMAKE_CURRENT_SOURCE_DIR}/.Build/${CMAKE_SYSTEM_NAME}/${ARCH}/Lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RAW "${CMAKE_CURRENT_SOURCE_DIR}/.Build/${CMAKE_SYSTEM_NAME}/${ARCH}/Lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "$<1:${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RAW}/>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "$<1:${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RAW}/>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "$<1:${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RAW}/>")
set( CMAKE_MSVC_RUNTIME_LIBRARY 		"MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" )
if (MSVC)
    set(CMAKE_C_FLAGS     	   "${CMAKE_C_FLAGS} /W3" )
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_C_FLAGS_DEBUG     	   "${CMAKE_CXX_FLAGS_DEBUG} /Zi" )
endif()
add_compile_definitions(
    "_CRT_SECURE_NO_DEPRECATE"
    "_CRT_STDIO_INLINE=__declspec(dllexport)__inline"
)

option(BUILD_SHARED_LIBS On)
option(BUILD_AMALGDLL Off)

set(DASMDIR "${CMAKE_CURRENT_SOURCE_DIR}/dynasm")
set(DASM "${DASMDIR}/dynasm.lua")
if (WIN32)
    set(DASMFLAGS "-D WIN -D JIT -D FFI")
    if ("${ARCH}" STREQUAL "x64")
        set(DASMFLAGS "${DASMFLAGS} -D P64")
    endif()
endif()

# Minilua generates a few things for the other libraries/executables.
file(GLOB ALL_LUA_LIB ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_*.c)
file(GLOB lj_files ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_*.c)
set(LUA_LIB
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_base.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_math.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_bit.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_string.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_table.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_io.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_os.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_package.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_debug.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_jit.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_ffi.c
)
add_executable(minilua ${CMAKE_CURRENT_SOURCE_DIR}/src/host/minilua.c)
if (WIN32)
    set(minilua_EXECUTABLE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RAW}/minilua.exe)
endif()
set(miniluacmd "${minilua_EXECUTABLE} ${DASM} -LN ${DASMFLAGS} -o ${CMAKE_CURRENT_SOURCE_DIR}/src/host/buildvm_arch.h ${CMAKE_CURRENT_SOURCE_DIR}/src/vm_x86.dasc")
if (WIN32)
    string(REPLACE "/" "\\" miniluacmd ${miniluacmd})
    separate_arguments(miniluacmd WINDOWS_COMMAND ${miniluacmd})
endif()
add_custom_command(TARGET minilua
	POST_BUILD
    COMMAND ${minilua_EXECUTABLE} || (set ERRORLEVEL=0) # Bypass
	COMMAND ${miniluacmd}
    COMMENT "${miniluacmd}"
)

# buildVM requires minilua to exist and to generate some files.
file(GLOB buildvm_files ${CMAKE_CURRENT_SOURCE_DIR}/src/host/buildvm*.c)
add_executable(buildvm ${buildvm_files})
target_include_directories(buildvm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dynasm ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_dependencies(buildvm minilua)
add_custom_command(TARGET buildvm
	POST_BUILD
	COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RAW}/buildvm.exe -m peobj -o lj_vm.obj
	COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RAW}/buildvm.exe -m bcdef -o ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_bcdef.h ${LUA_LIB}
	COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RAW}/buildvm.exe -m ffdef -o ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_ffdef.h ${LUA_LIB}
	COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RAW}/buildvm.exe -m libdef -o ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_libdef.h ${LUA_LIB}
	COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RAW}/buildvm.exe -m recdef -o ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_recdef.h ${LUA_LIB}
	COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RAW}/buildvm.exe -m vmdef -o ${CMAKE_CURRENT_SOURCE_DIR}/src/jit/lj_vmdef.lua ${LUA_LIB}
	COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RAW}/buildvm.exe -m folddef -o ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_folddef.h ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_opt_fold.c
)


# Lua51 requires buildVM to do some generation first.
if (BUILD_AMALGDLL)
    add_library(lua51 SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/ljamalg.c ${CMAKE_BINARY_DIR}/lj_vm.obj)
    target_compile_definitions(lua51 LUA_BUILD_AS_DLL)
elseif (BUILD_SHARED_LIBS)
    add_library(lua51 SHARED ${ALL_LUA_LIB} ${lj_files})
    target_compile_definitions(lua51 LUA_BUILD_AS_DLL)
else()
    add_library(lua51 STATIC ${ALL_LUA_LIB} ${lj_files})
endif()
add_dependencies(lua51 buildvm)
target_include_directories(lua51 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR})
set_target_properties(lua51 PROPERTIES LINKER_FLAGS "/nodefaultlib")

# LuaJit requires Lua51 to exist and buildVM to finish generation.
add_executable(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/luajit.c
)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PUBLIC lua51)
