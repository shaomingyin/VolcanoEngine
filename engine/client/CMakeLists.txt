
cmake_dependent_option(VOLCANO_CLIENT "Enable client" ON "VOLCANO_GUI;VOLCANO_GAME" OFF)
if(NOT VOLCANO_CLIENT)
    return()
endif()

include(FindGLFW)
if(NOT GLFW_FOUND)
    message(FATAL_ERROR "GLFW not found.")
endif()

configure_file(
    include/Volcano/Client/Config.hpp.in
    include/Volcano/Client/Config.hpp
    )

file(GLOB_RECURSE VOLCANO_CLIENT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

volcano_nodejs_native_module(client
    DPEENDENCIES
        graphics
        sound
    PRIVATE_LINK_LIBRARIES
        graphics
        sound
        ${GLFW_LIBRARIES}
    PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
        ${GLFW_INCLUDE_DIRS}
    PRIVATE_SOURCES
        ${VOLCANO_CLIENT_SOURCES}
    )
