#
# Compiling Options
#
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "$<1:${CMAKE_CURRENT_SOURCE_DIR}/.Build/Bin/>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "$<1:${CMAKE_CURRENT_SOURCE_DIR}/.Build/Lib/>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "$<1:${CMAKE_CURRENT_SOURCE_DIR}/.Build/Lib/>")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

message("CMake Version 					: 	${CMAKE_VERSION}")
message("Config types 					: 	${CMAKE_CONFIGURATION_TYPES}")
message("CompileOptions Configuration 	: 	${CMAKE_BUILD_TYPE}")
message("CompileOptions Platform 		: 	${CMAKE_SYSTEM_NAME}")
message("CompileOptions Compiler 		: 	${CMAKE_CXX_COMPILER_ID}")
message("CompileOptions Generator 		: 	${CMAKE_GENERATOR}")

if (CLANG OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	message("CompileOptions [Clang] : ${CMAKE_CXX_COMPILER_VERSION}")

	# Every warning is an Error.
	add_definitions(
		-Wall -Wextra -Wpedantic -Werror
	)

	set( CMAKE_CXX_FLAGS_SHIPPING  			"${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_SHIPPING}" )
	set( CMAKE_CXX_FLAGS_RELEASE   			"${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} " )
	set( CMAKE_CXX_FLAGS_DEBUG     			"${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}" )

elseif (MSVC)
	# set(CMAKE_EXE_LINKER_FLAGS 				"${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB")

	# Runtime lib
	set( CMAKE_MSVC_RUNTIME_LIBRARY 		"MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" )

	# Every warning is an Error.
	set( CMAKE_CXX_FLAGS  					"${CMAKE_CXX_FLAGS} /W4 /WX" )

	# General optimization
	set( CMAKE_CXX_FLAGS_SHIPPING  			"${CMAKE_CXX_FLAGS_SHIPPING} /Ox" )
	set( CMAKE_CXX_FLAGS_RELEASE   			"${CMAKE_CXX_FLAGS_RELEASE} /O2" )
	set( CMAKE_CXX_FLAGS_DEBUG     			"${CMAKE_CXX_FLAGS_DEBUG} /Od" )

	# Debug information
	# set( CMAKE_CXX_FLAGS_SHIPPING  			"${CMAKE_CXX_FLAGS_SHIPPING} /Zi" )
	set( CMAKE_CXX_FLAGS_RELEASE   			"${CMAKE_CXX_FLAGS_RELEASE} /Zi" )
	set( CMAKE_CXX_FLAGS_DEBUG     			"${CMAKE_CXX_FLAGS_DEBUG} /Zi" )

	# Just-My-Code debugging
	# set( CMAKE_CXX_FLAGS_SHIPPING  			"${CMAKE_CXX_FLAGS_SHIPPING} /JMC-" )
	set( CMAKE_CXX_FLAGS_RELEASE   			"${CMAKE_CXX_FLAGS_RELEASE} /JMC-" )
	set( CMAKE_CXX_FLAGS_DEBUG     			"${CMAKE_CXX_FLAGS_DEBUG} /JMC" )

	# Function Level Linking
	set( CMAKE_CXX_FLAGS_SHIPPING  			"${CMAKE_CXX_FLAGS_SHIPPING} /Gy-" )
	set( CMAKE_CXX_FLAGS_RELEASE   			"${CMAKE_CXX_FLAGS_RELEASE} /Gy" )
	set( CMAKE_CXX_FLAGS_DEBUG     			"${CMAKE_CXX_FLAGS_DEBUG} /Gy" )
else()
	message(FATAL_ERROR "${CMAKE_CXX_COMPILER_ID} compiler is not supported!")
endif()