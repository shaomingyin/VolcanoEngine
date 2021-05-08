
configure_file(
    include/Volcano/Input/Config.hpp.in
    include/Volcano/Input/Config.hpp
    )

file(GLOB_RECURSE VOLCANO_INPUT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

volcano_static_library(input
    DEPENDENCIES
        base
    PUBLIC_LINK_LIBRARIES
        base
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        ${VOLCANO_INPUT_SOURCES}
    )
