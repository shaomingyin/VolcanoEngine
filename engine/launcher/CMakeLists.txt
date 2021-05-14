
cmake_dependent_option(VOLCANO_LAUNCHER "Enable launcher" ON "VOLCANO_GUI;VOLCANO_GAME;VOLCANO_RPC" OFF)
if(NOT VOLCANO_LAUNCHER)
    return()
endif()

include(FindGLFW)
if(NOT GLFW_FOUND)
    message(FATAL_ERROR "GLFW not found.")
endif()

set(VOLCANO_LAUNCHER_ENABLE_MULTIPLAYER OFF CACHE BOOL "Enable multiplayer functions.")

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
        server
        graphics
        input
        sound
    PRIVATE_LINK_LIBRARIES
        graphics
        input
        sound
        rpc
        ${GLFW_LIBRARIES}
    PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
        ${GLFW_INCLUDE_DIRS}
    PRIVATE_SOURCES
        ${VOLCANO_LAUNCHER_SOURCES}
    )
