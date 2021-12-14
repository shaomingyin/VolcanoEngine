
macro(volcano_native_target_setup TARGET)
    set(OPTIONS
        DISABLE_AUTO_SOURCE_GROUP
        )

    set(ONE_VALUE_ARGS
        FOLDER
        )

    set(MULTI_VALUE_ARGS
        DEPENDENCIES
        PUBLIC_SOURCES
        PRIVATE_SOURCES
        INTERFACE_SOURCES
        PUBLIC_DEFINITIONS
        PRIVATE_DEFINITIONS
        INTERFACE_DEFINITIONS
        PUBLIC_INCLUDE_DIRECTORIES
        PRIVATE_INCLUDE_DIRECTORIES
        INTERFACE_INCLUDE_DIRECTORIES
        PUBLIC_LINK_DIRECTORIES
        PRIVATE_LINK_DIRECTORIES
        PUBLIC_LINK_LIBRARIES
        PRIVATE_LINK_LIBRARIES
        INTERFACE_LINK_LIBRARIES
        )

    cmake_parse_arguments(${TARGET} "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN})

    set_target_properties(${TARGET} PROPERTIES
        OUTPUT_NAME volcano-${TARGET}
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )

    if(${TARGET}_FOLDER)
        set_target_properties(${TARGET} PROPERTIES FOLDER ${${TARGET}_FOLDER})
    endif()

    if(${TARGET}_DEPENDENCIES)
        add_dependencies(${TARGET} ${${TARGET}_DEPENDENCIES})
    endif()

    foreach(PUBLIC_SOURCE ${${TARGET}_PUBLIC_SOURCES})
        file(REAL_PATH ${PUBLIC_SOURCE} PUBLIC_SOURCE_FULL_PATH)
        target_sources(${TARGET} PUBLIC ${PUBLIC_SOURCE_FULL_PATH})
    endforeach()

    foreach(PRIVATE_SOURCE ${${TARGET}_PRIVATE_SOURCES})
        file(REAL_PATH ${PRIVATE_SOURCE} PRIVATE_SOURCE_FULL_PATH)
        target_sources(${TARGET} PRIVATE ${PRIVATE_SOURCE_FULL_PATH})
    endforeach()

    foreach(INTERFACE_SOURCE ${${TARGET}_INTERFACE_SOURCES})
        file(REAL_PATH ${INTERFACE_SOURCE} INTERFACE_SOURCE_FULL_PATH)
        target_sources(${TARGET} INTERFACE ${INTERFACE_SOURCE_FULL_PATH})
    endforeach()

    foreach(PUBLIC_DEFINITION ${${TARGET}_PUBLIC_DEFINITIONS})
        target_compile_definitions(${TARGET} PUBLIC ${PUBLIC_DEFINITION})
    endforeach()

    foreach(PRIVATE_DEFINITION ${${TARGET}_PRIVATE_DEFINITIONS})
        target_compile_definitions(${TARGET} PRIVATE ${PRIVATE_DEFINITION})
    endforeach()

    foreach(INTERFACE_DEFINITION ${${TARGET}_INTERFACE_DEFINITIONS})
        target_compile_definitions(${TARGET} INTERFACE ${INTERFACE_DEFINITION})
    endforeach()

    foreach(PUBLIC_INCLUDE_DIRECTORY ${${TARGET}_PUBLIC_INCLUDE_DIRECTORIES})
        file(REAL_PATH ${PUBLIC_INCLUDE_DIRECTORY} PUBLIC_INCLUDE_DIRECTORY_FULL_PATH)
        target_include_directories(${TARGET} PUBLIC ${PUBLIC_INCLUDE_DIRECTORY_FULL_PATH})
    endforeach()

    foreach(PRIVATE_INCLUDE_DIRECTORY ${${TARGET}_PRIVATE_INCLUDE_DIRECTORIES})
        file(REAL_PATH ${PRIVATE_INCLUDE_DIRECTORY} PRIVATE_INCLUDE_DIRECTORY_FULL_PATH)
        target_include_directories(${TARGET} PRIVATE ${PRIVATE_INCLUDE_DIRECTORY_FULL_PATH})
    endforeach()

    foreach(INTERFACE_INCLUDE_DIRECTORY ${${TARGET}_INTERFACE_INCLUDE_DIRECTORIES})
        file(REAL_PATH ${INTERFACE_INCLUDE_DIRECTORY} INTERFACE_INCLUDE_DIRECTORY_FULL_PATH)
        target_include_directories(${TARGET} INTERFACE ${INTERFACE_INCLUDE_DIRECTORY_FULL_PATH})
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

    foreach(INTERFACE_LINK_LIBRARY ${${TARGET}_INTERFACE_LINK_LIBRARIES})
        target_link_libraries(${TARGET} INTERFACE ${INTERFACE_LINK_LIBRARY})
    endforeach()

    if(NOT ${TARGET}_DISABLE_AUTO_SOURCE_GROUP)
        source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
            ${${TARGET}_PUBLIC_SOURCES}
            ${${TARGET}_PRIVATE_SOURCES}
            )
    endif()
endmacro()

macro(volcano_interface TARGET)
    add_library(${TARGET} INTERFACE)
    volcano_native_target_setup(${TARGET} ${ARGN})
endmacro()

macro(volcano_static_library TARGET)
    add_library(${TARGET} STATIC)
    volcano_native_target_setup(${TARGET} ${ARGN})
endmacro()

macro(volcano_shared_library TARGET)
    add_library(${TARGET} SHARED)
    volcano_native_target_setup(${TARGET} ${ARGN})
endmacro()

macro(volcano_executable TARGET)
    add_executable(${TARGET})
    volcano_native_target_setup(${TARGET} ${ARGN})
endmacro()
