
# BulletPhysics
include(FindBullet)
if(NOT BULLET_FOUND)
    message(FATAL_ERROR "BulletPhysics not found.")
endif()

file(GLOB_RECURSE VOLCANO_GAME_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_library(game STATIC ${VOLCANO_GAME_SOURCES})
set_target_properties(game PROPERTIES OUTPUT_NAME "VolcanoGame")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VOLCANO_GAME_SOURCES})

target_include_directories(game
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
        ${BULLET_INCLUDE_DIR}
    )

target_link_libraries(game
    PUBLIC
        base
        ${BULLET_LIBRARIES}
    )

configure_file(
    include/Volcano/Game/Config.hpp.in
    include/Volcano/Game/Config.hpp
    )
