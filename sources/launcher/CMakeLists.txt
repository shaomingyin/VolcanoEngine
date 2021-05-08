
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
    PRIVATE_DEFINITIONS
        SDL_MAIN_HANDLED
    PRIVATE_SOURCES
        ${VOLCANO_LAUNCHER_SOURCES}
    )
