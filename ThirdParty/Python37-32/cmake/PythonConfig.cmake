
set(PYTHON_ROOT_DIR "${PROJECT_SOURCE_DIR}/ThirdParty/Python37-32/${CMAKE_SYSTEM_NAME}")
set(Python_EXECUTABLE "${PYTHON_ROOT_DIR}/python.exe")
set(Python_INCLUDE_DIRS "${PYTHON_ROOT_DIR}/include")
set(Python_LIBRARIES "${PYTHON_ROOT_DIR}/libs")
if (WIN32)
    add_compile_definitions(MS_NO_COREDLL)
    remove_definitions( -DTIME_WITH_SYS_TIME -DHAVE_SYS_TIME_H )
endif()

add_library(Python::Link STATIC IMPORTED GLOBAL)
set_target_properties(Python::Link PROPERTIES
    IMPORTED_LOCATION_DEBUG "${Python_LIBRARIES}/python37.lib"
    IMPORTED_LOCATION_DEBUGNOASSERT "${Python_LIBRARIES}/python37.lib"
    IMPORTED_LOCATION_RELEASE "${Python_LIBRARIES}/python37.lib"
    IMPORTED_LOCATION_SHIPPING "${Python_LIBRARIES}/python37.lib"
)
function(python_add_library target_name)
    cmake_parse_arguments(PARSE_ARGV 1 ARG
        "STATIC;SHARED;MODULE;THIN_LTO;OPT_SIZE;NO_EXTRAS;WITHOUT_SOABI" "" "")

    if(ARG_STATIC)
        set(lib_type STATIC)
    elseif(ARG_SHARED)
        set(lib_type SHARED)
    else()
        set(lib_type MODULE)
    endif()

    add_library(${target_name} ${lib_type} ${ARG_UNPARSED_ARGUMENTS})
endfunction()
# set(PYTHON_MODULE_EXTENSION "")