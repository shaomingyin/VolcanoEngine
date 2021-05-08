
configure_file(
    include/Volcano/Sound/Config.hpp.in
    include/Volcano/Sound/Config.hpp
    )

file(GLOB_RECURSE VOLCANO_SOUND_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

volcano_static_library(sound
    DEPENDENCIES
        base
    PUBLIC_LINK_LIBRARIES
        base
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        ${VOLCANO_SOUND_SOURCES}
    )
