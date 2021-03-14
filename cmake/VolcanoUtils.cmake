
macro(volcano_target_setup TARGET)
    set(OPTIONS)

    set(ONE_VALUE_ARGS
        OUTPUT_NAME
        )

    set(MULTI_VALUE_ARGS
        DEPENDENCIES
        CONFIGURE_FILES
        PUBLIC_SOURCES
        PRIVATE_SOURCES
        PUBLIC_DEFINITIONS
        PRIVATE_DEFINITIONS
        PUBLIC_INCLUDE_DIRECTORIES
        PRIVATE_INCLUDE_DIRECTORIES
        PUBLIC_LINK_DIRECTORIES
        PRIVATE_LINK_DIRECTORIES
        PUBLIC_LINK_LIBRARIES
        PRIVATE_LINK_LIBRARIES
        )

    cmake_parse_arguments(${TARGET}
        "${OPTIONS}"
        "${ONE_VALUE_ARGS}"
        "${MULTI_VALUE_ARGS}"
        ${ARGN})

    if(${TARGET}_OUTPUT_NAME)
        set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME Volcano${${TARGET}_OUTPUT_NAME})
    endif()

    if(${TARGET}_DEPENDENCIES)
        add_dependencies(${TARGET} ${${TARGET}_DEPENDENCIES})
    endif()

    foreach(CONFIG_FILE ${${TARGET}_CONFIGURE_FILES})
        if(CONFIG_FILE MATCHES "\\.in\$")
            string(REGEX REPLACE "\\.in\$" "" CONFIGED_FILE ${CONFIG_FILE})
        else()
            set(CONFIGED_FILE ${CONFIG_FILE})
        endif()
        configure_file(${CONFIG_FILE} ${CONFIGED_FILE})
        target_sources(${TARGET} PRIVATE ${CONFIG_FILE})
    endforeach()

    foreach(PUBLIC_SOURCE ${${TARGET}_PUBLIC_SOURCES})
        target_sources(${TARGET} PUBLIC ${PUBLIC_SOURCE})
    endforeach()

    foreach(PRIVATE_SOURCE ${${TARGET}_PRIVATE_SOURCES})
        target_sources(${TARGET} PRIVATE ${PRIVATE_SOURCE})
    endforeach()

    foreach(PUBLIC_DEFINITION ${${TARGET}_PUBLIC_DEFINITIONS})
        target_compile_definitions(${TARGET} PUBLIC ${PUBLIC_DEFINITION})
    endforeach()

    foreach(PRIVATE_DEFINITION ${${TARGET}_PRIVATE_DEFINITIONS})
        target_compile_definitions(${TARGET} PRIVATE ${PRIVATE_DEFINITION})
    endforeach()

    string(TOUPPER ${TARGET} TARGET_UP)
    target_compile_definitions(${TARGET} PRIVATE VOLCANO_${TARGET_UP})

    foreach(PUBLIC_INCLUDE_DIRECTORY ${${TARGET}_PUBLIC_INCLUDE_DIRECTORIES})
        target_include_directories(${TARGET} PUBLIC ${PUBLIC_INCLUDE_DIRECTORY})
    endforeach()

    foreach(PRIVATE_INCLUDE_DIRECTORY ${${TARGET}_PRIVATE_INCLUDE_DIRECTORIES})
        target_include_directories(${TARGET} PRIVATE ${PRIVATE_INCLUDE_DIRECTORY})
    endforeach()

    foreach(PUBLIC_LINK_DIRECTORY ${${TARGET}_PUBLIC_LINK_DIRECOTRIES})
        target_link_directories(${TARGET} PUBLIC ${${TARGET}PUBLIC_LINK_DIRECTORY})
    endforeach()

    foreach(PRIVATE_LINK_DIRECTORY ${${TARGET}_PRIVATE_LINK_DIRECOTRIES})
        target_link_directories(${TARGET} PRIVATE ${PRIVATE_LINK_DIRECTORY})
    endforeach()

    foreach(PUBLIC_LINK_LIBRARY ${${TARGET}_PUBLIC_LINK_LIBRARIES})
        target_link_libraries(${TARGET} PUBLIC ${PUBLIC_LINK_LIBRARY})
    endforeach()

    foreach(PRIVATE_LINK_LIBRARY ${${TARGET}_PRIVATE_LINK_LIBRARIES})
        target_link_libraries(${TARGET} PRIVATE ${PRIVATE_LINK_LIBRARY})
    endforeach()

    get_target_property(${TARGET}_SOURCES ${TARGET} SOURCES)
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${TARGET}_SOURCES})
endmacro()

macro(volcano_library TARGET)
    add_library(${TARGET} STATIC)
    volcano_target_setup(${TARGET} ${ARGN})
endmacro()

macro(volcano_shared_library TARGET)
    add_library(${TARGET} SHARED)
    volcano_target_setup(${TARGET} ${ARGN})
endmacro()

macro(volcano_executable TARGET)
    add_executable(${TARGET})
    volcano_target_setup(${TARGET} ${ARGN})
endmacro()
