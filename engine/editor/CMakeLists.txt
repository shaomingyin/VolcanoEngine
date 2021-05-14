
cmake_dependent_option(VOLCANO_EDITOR "Enable editor" ON "VOLCANO_GUI;VOLCANO_GAME;VOLCANO_LAUNCHER;VOLCANO_SERVER" OFF)
if(NOT VOLCANO_EDITOR)
    return()
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

configure_file(
    include/Volcano/Editor/Config.hpp.in
    include/Volcano/Editor/Config.hpp
    )

file(GLOB_RECURSE VOLCANO_EDITOR_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

volcano_executable(editor
    DEPENDENCIES
        launcher
        server
    PRIVATE_LINK_LIBRARIES
        graphics
        input
        sound
        game
        base
        Qt5::Widgets
        Qt5::Gui
        Qt5::Core
    PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        ${VOLCANO_EDITOR_SOURCES}
    )

source_group(\\generated\\mocs REGULAR_EXPRESSION "mocs_.*\.cpp$")
source_group(\\generated\\qrc REGULAR_EXPRESSION "qrc_.*\.cpp$")
