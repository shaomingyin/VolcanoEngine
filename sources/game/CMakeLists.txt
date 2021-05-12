
option(VOLCANO_GAME "Enable game system" ON)
if(NOT VOLCANO_GAME)
    return()
endif()

include(FindBullet)
if(NOT BULLET_FOUND)
    message(FATAL_ERROR "BulletPhysics not found.")
endif()

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
        ${BULLET_LIBRARIES}
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
        ${BULLET_INCLUDE_DIRS}
    PRIVATE_SOURCES
        ${VOLCANO_GAME_SOURCES}
    )
