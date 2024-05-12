
macro(volcano_glob_sources VAR DIR)
    file(GLOB_RECURSE ${VAR} LIST_DIRECTORIES false
        ${DIR}/*.h
        ${DIR}/*.hpp
        ${DIR}/*.c
        ${DIR}/*.cpp
        ${DIR}/*.in
        ${DIR}/*.qrc
        ${DIR}/*.json
        ${DIR}/*.cmake
        ${DIR}/*.vpf
        ${DIR}/*.vwf
    )
endmacro()

macro(volcano_setup_target TARGET)
    set(FLAG_ARGS "")
    set(ONE_VALUE_ARGS
        OUTPUT_NAME
    )
    set(MULTI_VALUE_ARGS
        PUBLIC_SOURCES
        PRIVATE_SOURCES
        PUBLIC_INCLUDE_DIRECTORIES
        PRIVATE_INCLUDE_DIRECTORIES
        PUBLIC_DEFINITIONS
        PRIVATE_DEFINITIONS
        PUBLIC_LINK_DIRECTORIES
        PRIVATE_LINK_DIRECTORIES
        PUBLIC_LINK_LIBRARIES
        PRIVATE_LINK_LIBRARIES
        PUBLIC_QT_MODULES
        PRIVATE_QT_MODULES
    )
    cmake_parse_arguments(ARG "${FLAG_ARGS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN})
    if(DEFINED ARG_OUTPUT_NAME)
        set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${ARG_OUTPUT_NAME})
    endif()
    if(DEFINED ARG_PUBLIC_SOURCES)
        target_sources(${TARGET} PUBLIC ${ARG_PUBLIC_SOURCES})
    endif()
    if(DEFINED ARG_PRIVATE_SOURCES)
        target_sources(${TARGET} PRIVATE ${ARG_PRIVATE_SOURCES})
    endif()
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ARG_PUBLIC_SOURCES} ${ARG_PRIVATE_SOURCES})
    foreach(TARGET_SOURCE ${ARG_PUBLIC_SOURCES} ${ARG_PRIVATE_SOURCES})
        if(TARGET_SOURCE MATCHES "\\.in$")
            file(REAL_PATH ${TARGET_SOURCE} TARGET_SOURCE_TEMPLATE)
            file(RELATIVE_PATH TARGET_SOURCE_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR} ${TARGET_SOURCE_TEMPLATE})
            string(REGEX REPLACE "\\.in$" "" TARGET_SOURCE_TEMPLATE_OUTPUT ${TARGET_SOURCE_TEMPLATE})
			message(STATUS "Configuring ${TARGET_SOURCE_TEMPLATE} to ${TARGET_SOURCE_TEMPLATE_OUTPUT}...")
            configure_file(${TARGET_SOURCE_TEMPLATE} ${TARGET_SOURCE_TEMPLATE_OUTPUT})
        endif()
    endforeach()
    foreach(PUBLIC_INCLUDE_DIRECTORY ${ARG_PUBLIC_INCLUDE_DIRECTORIES})
            target_include_directories(${TARGET} PUBLIC ${PUBLIC_INCLUDE_DIRECTORY})
    endforeach()
    foreach(PRIVATE_INCLUDE_DIRECTORY ${ARG_PRIVATE_INCLUDE_DIRECTORIES})
        target_include_directories(${TARGET} PRIVATE ${PRIVATE_INCLUDE_DIRECTORY})
    endforeach()
    foreach(PUBLIC_DEFINITION ${ARG_PUBLIC_DEFINITIONS})
        target_compile_definitions(${TARGET} PUBLIC ${PUBLIC_DEFINITION})
    endforeach()
    foreach(PRIVATE_DEFINITION ${ARG_PRIVATE_DEFINITIONS})
        target_compile_definitions(${TARGET} PRIVATE ${PRIVATE_DEFINITION})
    endforeach()
    foreach(PUBLIC_LINK_DIRECTORY ${ARG_PUBLIC_LINK_DIRECTORIES})
        target_link_directories(${TARGET} PUBLIC ${PUBLIC_LINK_DIRECTORY})
    endforeach()
    foreach(PRIVATE_LINK_DIRECTORY ${ARG_PRIVATE_LINK_DIRECTORIES})
        target_link_directories(${TARGET} PRIVATE ${PRIVATE_LINK_DIRECTORY})
    endforeach()
    foreach(PUBLIC_LINK_LIBRARY ${ARG_PUBLIC_LINK_LIBRARIES})
        target_link_libraries(${TARGET} PUBLIC ${PUBLIC_LINK_LIBRARY})
    endforeach()
    foreach(PRIVATE_LINK_LIBRARY ${ARG_PRIVATE_LINK_LIBRARIES})
        target_link_libraries(${TARGET} PRIVATE ${PRIVATE_LINK_LIBRARY})
    endforeach()
    foreach(PUBLIC_QT_MODULE ${ARG_PUBLIC_QT_MODULES})
        find_package(Qt6 COMPONENTS ${PUBLIC_QT_MODULE} REQUIRED)
        target_link_libraries(${TARGET} PUBLIC Qt6::${PUBLIC_QT_MODULE})
    endforeach()
    foreach(PRIVATE_QT_MODULE ${ARG_PRIVATE_QT_MODULES})
        find_package(Qt6 COMPONENTS ${PRIVATE_QT_MODULE} REQUIRED)
        target_link_libraries(${TARGET} PRIVATE Qt6::${PRIVATE_QT_MODULE})
    endforeach()
endmacro()

macro(volcano_define_static_library TARGET)
    add_library(${TARGET} STATIC)
    volcano_setup_target(${TARGET} ${ARGN})
endmacro()

macro(volcano_define_shared_library TARGET)
    add_library(${TARGET} SHARED)
    volcano_setup_target(${TARGET} ${ARGN})
endmacro()

macro(volcano_define_executable TARGET)
    add_executable(${TARGET})
    volcano_setup_target(${TARGET} ${ARGN})
endmacro()
