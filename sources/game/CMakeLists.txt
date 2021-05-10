
option(VOLCANO_GAME "Enable game system" ON)

configure_file(
    include/Volcano/Game/Config.hpp.in
    include/Volcano/Game/Config.hpp
    )

file(GLOB_RECURSE VOLCANO_GAME_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

volcano_static_library(game
    PUBLIC_LINK_LIBRARIES
        base
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        ${VOLCANO_GAME_SOURCES}
    )
