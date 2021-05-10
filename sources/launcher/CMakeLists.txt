
cmake_dependent_option(VOLCANO_LAUNCHER "Enable launcher" ON "VOLCANO_GUI;VOLCANO_GAME;VOLCANO_RPC" OFF)
if(NOT VOLCANO_LAUNCHER)
    return()
endif()

include(FindSDL2)
if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 not found.")
endif()

configure_file(
    include/Volcano/Launcher/Config.hpp.in
    include/Volcano/Launcher/Config.hpp
    )

file(GLOB_RECURSE VOLCANO_LAUNCHER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

volcano_executable(launcher
    DPEENDENCIES
        game
        graphics
        input
        sound
        rpc
    PRIVATE_LINK_LIBRARIES
        game
        graphics
        input
        sound
        rpc
        ${SDL2_LIBRARY}
    PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
        ${SDL2_INCLUDE_DIR}
    PRIVATE_SOURCES
        ${VOLCANO_LAUNCHER_SOURCES}
    )
