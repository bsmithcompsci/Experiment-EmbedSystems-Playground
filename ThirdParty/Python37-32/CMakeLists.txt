# set(PYBIND11_INTERNALPYTHON ON) # Pybind 11 module related.

include(cmake/PythonConfig.cmake)


# set(PYTHON_EXECUTABLE "${CMAKE_CURRENT_LIST_DIR}/${Python_Platform_Directory}/python.exe")
# set(PYTHON_INCLUDE_DIR "")

# set(Python_Platform_Directory "${CMAKE_SYSTEM_NAME}")
# file(REAL_PATH "${CMAKE_CURRENT_LIST_DIR}/${Python_Platform_Directory}" Python_Platform_Directory)

# include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

# if (CMAKE_BUILD_TYPE MATCHES "Debug")
#   set(CPYTHON_BUILD_TYPE Debug)
# else ()
#   set(CPYTHON_BUILD_TYPE Release)
# endif()

# # Add the cpython as an external project that will be included in the build
# if(MSVC)
#   if(CMAKE_CL_64)
#     set(CPYTHON_PLATFORM x64)
#     set(CPYTHON_BUILD_DIR amd64)
#   else()
#     set(CPYTHON_PLATFORM x86)
#     set(CPYTHON_BUILD_DIR win32)
#   endif()
#   ExternalProject_Add(CPYTHON
#     DOWNLOAD_COMMAND ""
#     SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/cpython
#     CONFIGURE_COMMAND ""
#     BUILD_COMMAND cd ${CMAKE_CURRENT_LIST_DIR}/cpython && MSBuild.exe /p:Configuration=${CPYTHON_BUILD_TYPE} /property:Platform=${CPYTHON_PLATFORM} "PCBuild/python.vcxproj" /nologo /verbosity:minimal /consoleloggerparameters:summar
#     INSTALL_COMMAND ""
#     TEST_COMMAND ""
#   )
# else()
#   ExternalProject_Add(CPYTHON
#     DOWNLOAD_COMMAND ""
#     SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/cpython
#     CONFIGURE_COMMAND cd ${CMAKE_CURRENT_LIST_DIR}/cpython && ./configure --disable-static --enable-shared 
#     BUILD_COMMAND cd ${CMAKE_CURRENT_LIST_DIR}/cpython && make
#     INSTALL_COMMAND ""
#     TEST_COMMAND ""
#   )
# endif()

# set(CPYTHON_STDLIB_DIR ${CMAKE_CURRENT_LIST_DIR}/cpython/Lib)
# if(MSVC)
#   set(CPYTHON_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/cpython/Include ${CMAKE_CURRENT_LIST_DIR}/cpython/PC)
#   set(CPYTHON_LIBRARY_DIR ${CMAKE_CURRENT_LIST_DIR}/cpython/PCBuild/${CPYTHON_BUILD_DIR})
#   if(CMAKE_BUILD_TYPE MATCHES "Debug")
#     set(CPYTHON_BIN ${CMAKE_CURRENT_LIST_DIR}/cpython/PCBuild/${CPYTHON_BUILD_DIR}/python38_d.dll)
#   else()
#     set(CPYTHON_BIN ${CMAKE_CURRENT_LIST_DIR}/cpython/PCBuild/${CPYTHON_BUILD_DIR}/python38.dll)
#   endif()
# else()
#   set(CPYTHON_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/cpython/Include ${CMAKE_CURRENT_LIST_DIR}/cpython)
#   set(CPYTHON_LIBRARY_DIR ${CMAKE_CURRENT_LIST_DIR}/cpython)
#   set(CPYTHON_LIBRARY python3.8m)
#   set(CPYTHON_BIN ${CMAKE_CURRENT_LIST_DIR}/cpython/libpython3.8m.so)
# endif()

# add_library(
#     Scripting_Python_Engine STATIC IMPORTED GLOBAL
# )

# target_link_directories(Scripting_Python_Engine INTERFACE ${CPYTHON_LIBRARY_DIR})
# target_include_directories(Scripting_Python_Engine INTERFACE ${CPYTHON_INCLUDE_DIR})
# target_include_directories(Scripting_Python_Engine INTERFACE ${PYBIND_INCLUDE_DIR})
# if(NOT MSVC)
#   target_link_libraries(${PROJECT_NAME} ${CPYTHON_LIBRARY})
# endif()

# target_include_directories( Scripting_Python_Engine INTERFACE "Source/Include" )