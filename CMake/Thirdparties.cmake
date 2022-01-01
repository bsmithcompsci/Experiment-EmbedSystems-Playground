#
# ThirdParties
#

## Lua ##
include(ThirdParty/_CMake/Lua.cmake)
include(ThirdParty/_CMake/LuaJIT.cmake)

## Python ##
set(pyroot "${PROJECT_SOURCE_DIR}/ThirdParty/Python37-32/${CMAKE_SYSTEM_NAME}")
set(PYTHON_EXECUTABLE "${pyroot}/python.exe")
add_subdirectory( ThirdParty/pybind11 )

## CSharp ##
include(ThirdParty/_CMake/Mono.cmake)

## Julia ##
# include(ThirdParty/_CMake/Julia.cmake)


## Java ##
# include(ThirdParty/_CMake/OpenJDK.cmake)

## Javascript ##
include(ThirdParty/_CMake/V8.cmake)
