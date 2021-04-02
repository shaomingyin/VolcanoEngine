
# OpenGL
include(FindOpenGL)
if(NOT OPENGL_FOUND)
    message(FATAL_ERROR "OpenGL not found.")
endif()

file(GLOB_RECURSE DRIVERS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

volcano_static_library(drivers
    DEPENDENCIES
        base
    PUBLIC_LINK_LIBRARIES
        base
        ${OPENGL_LIBRARIES}
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
        ${OPENGL_INCLUDE_DIR}
    PRIVATE_SOURCES
        ${DRIVERS_SOURCES}
    )

configure_file(
    include/Volcano/Drivers/Config.hpp.in
    include/Volcano/Drivers/Config.hpp
    )
