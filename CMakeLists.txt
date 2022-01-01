#
# Establish Version Limits to a minimum of 3.17 for CMake's Build Systems.
#
cmake_minimum_required( VERSION 3.17 )
#
# Establish this Project
#
project( Embed-Systems LANGUAGES CXX C )

include_directories( include )

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Finds)

include( CMAKE/GlobalVars.cmake )
include( CMake/Functions.cmake )
include( CMake/CompileOptions.cmake )
include( CMake/ThirdParties.cmake )
include( CMake/Targets.cmake )

if (WIN32)
	set( CMAKE_USE_SCHANNEL ON )
else()
	set( CMAKE_USE_OPENSSL ON )
endif()

#
# Executables
#
# Ensure we link everything together correctly.
foreach(language ${LANGUAGE_EXECUTABLES})
	set(language_executable Test-${language})

	if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/${language}/main.cpp")
		message(FATAL_ERROR "Failed to setup language executable: Create a `${CMAKE_CURRENT_SOURCE_DIR}/src/${language}/main.cpp`")
	endif()

	add_executable( ${language_executable}
		src/${language}/main.cpp
	)

	target_link_libraries(${language_executable} ${${PROJECT_NAME}_${language}_LIBRARIES} Utils )

	# Set settings to fit MSVC and make the IDE more visible.
	set_target_properties( ${${PROJECT_NAME}_${language}_LIBRARIES} PROPERTIES FOLDER "Scripting Support" )
	if(MSVC)
		set_target_properties( ${language_executable} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE" )
	endif()
	set_target_cwd(${language_executable} ${PROJECT_SOURCE_DIR}/.Build/Bin)
	message("Setup Language Target: ${language}")
endforeach()

# Take whatever last the variable was at and make that our main project.
set_startup_target( ${language_executable} )

# Move the scripts into the build once built.
library_post_installbuild( Embed-Systems ${CMAKE_SOURCE_DIR}/scripts ${PROJECT_SOURCE_DIR}/.Build/Bin/scripts )