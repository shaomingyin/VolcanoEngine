
cmake_dependent_option(VOLCANO_SERVER "Enable server" ON "VOLCANO_RPC;VOLCANO_GAME" OFF)
if(NOT VOLCANO_SERVER)
    return()
endif()

include(FindUV)
if(NOT UV_FOUND)
    message(FATAL_ERROR "libuv not found.")
endif()

configure_file(
    include/Volcano/Server/Config.hpp.in
    include/Volcano/Server/Config.hpp
    )

file(GLOB_RECURSE VOLCANO_SERVER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

volcano_executable(server
    DEPENDENCIES
        game
        rpc
        base
    PRIVATE_LINK_LIBRARIES
        game
        rpc
        base
        ${UV_LIBRARIES}
    PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
        ${UV_INCLUDE_DIRS}
    PRIVATE_SOURCES
        ${VOLCANO_SERVER_SOURCES}
    )
