
option(VOLCANO_GRAPHICS "Enable graphics system." ON)
if(NOT VOLCANO_GRAPHICS)
    return()
endif()

configure_file(
    include/Volcano/Graphics/Config.hpp.in
    include/Volcano/Graphics/Config.hpp
    )

file(GLOB_RECURSE VOLCANO_GRAPHICS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

volcano_static_library(graphics
    DEPENDENCIES
        base
    PUBLIC_LINK_LIBRARIES
        base
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        ${VOLCANO_GRAPHICS_SOURCES}
    )
