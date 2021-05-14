
cmake_dependent_option(VOLCANO_GUI "Enable gui system" ON "VOLCANO_GRAPHICS;VOLCANO_INPUT;VOLCANO_SOUND" OFF)
if(NOT VOLCANO_GUI)
    return()
endif()

configure_file(
    include/Volcano/Gui/Config.hpp.in
    include/Volcano/Gui/Config.hpp
    )

file(GLOB_RECURSE VOLCANO_GUI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

volcano_static_library(gui
    DEPENDENCIES
        graphics
        input
        sound
        base
    PUBLIC_LINK_LIBRARIES
        graphics
        input
        sound
        base
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        ${VOLCANO_GUI_SOURCES}
    )
