
configure_file(
    include/Volcano/Config.hpp.in
    include/Volcano/Config.hpp
    )

file(GLOB_RECURSE VOLCANO_BASE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

volcano_static_library(base
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        ${VOLCANO_BASE_SOURCES}
    )
