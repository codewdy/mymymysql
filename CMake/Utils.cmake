MACRO (new_executable project)
    AUX_SOURCE_DIRECTORY(. SRC_LIST)
    ADD_EXECUTABLE(${project} ${SRC_LIST})
ENDMACRO ()

MACRO (new_static_library project)
    AUX_SOURCE_DIRECTORY(. SRC_LIST)
    ADD_LIBRARY(${project} STATIC ${SRC_LIST})
ENDMACRO ()

MACRO (new_shared_library project)
    AUX_SOURCE_DIRECTORY(. SRC_LIST)
    ADD_LIBRARY(${project} SHARED ${SRC_LIST})
ENDMACRO ()

MACRO (new_module_library project)
    AUX_SOURCE_DIRECTORY(. SRC_LIST)
    ADD_LIBRARY(${project} MODULE ${SRC_LIST})
ENDMACRO ()

IF (${UNIX})
    SET(CMD_MV mv)
    SET(CMD_CP cp)
    SET(CMD_RM rm)
    SET(CMD_IG cat)
ELSE ()
    SET(CMD_MV move /y)
    SET(CMD_CP copy /y)
    SET(CMD_RM del)
    SET(CMD_IG echo off)
ENDIF()

MACRO (new_copy_only dst file)
    IF (NOT ("${EXECUTABLE_OUTPUT_PATH}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}"))
        ADD_CUSTOM_COMMAND(OUTPUT "${EXECUTABLE_OUTPUT_PATH}/${file}"
            COMMAND ${CMD_CP} ${file} "${EXECUTABLE_OUTPUT_PATH}/${file}"
            MAIN_DEPENDENCY ${file}
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            )
        ADD_CUSTOM_TARGET(${dst} SOURCES "${EXECUTABLE_OUTPUT_PATH}/${file}")
    ENDIF()
ENDMACRO()

