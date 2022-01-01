function(library_post_installbuild NAME TARGET_FILE DEST_FILE)
    if (NOT PARALLEL_BUILD)
        if (NOT EXISTS ${DEST_FILE})
            make_directory(${DEST_FILE})
        endif()
        if (WILL_COPY)
            if (EXISTS "${TARGET_FILE}/") # Directory Copy
                execute_process(
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${TARGET_FILE} ${DEST_FILE}
                )
            else() # File Copy
                execute_process(
                    COMMAND ${CMAKE_COMMAND} -E copy ${TARGET_FILE} ${DEST_FILE}
                )
            endif()
        endif()
        if (TARGET ${NAME})
            get_target_property(type ${NAME} TYPE)
            get_target_property(imported ${NAME} IMPORTED)
            if (NOT ${type} STREQUAL "INTERFACE_LIBRARY" AND NOT ${imported})
                if (EXISTS "${TARGET_FILE}/") # Directory Copy
                    add_custom_command(TARGET ${NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_directory ${TARGET_FILE} ${DEST_FILE}
                    )
                else() # File Copy
                    add_custom_command(TARGET ${NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy ${TARGET_FILE} ${DEST_FILE}
                    )
                endif()
            endif()
        endif()
    endif()
endfunction()

macro(add_linktoexe NAME)
    set(TARGET_NAME "${NAME}")
    string(REPLACE "-" "" TARGET_NAME ${TARGET_NAME})
    string(REPLACE "Scripting" "" TARGET_NAME ${TARGET_NAME})

    list(APPEND LANGUAGE_EXECUTABLES ${TARGET_NAME})
    list(APPEND ${PROJECT_NAME}_${TARGET_NAME}_LIBRARIES ${NAME})
endmacro()

function(set_startup_target NAME)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${NAME})
endfunction()

function(set_target_cwd NAME DIRECTORY)
    set_property(TARGET ${NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${DIRECTORY}")
endfunction()