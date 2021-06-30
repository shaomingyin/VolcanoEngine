
file(GLOB_RECURSE GAME_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_library(game STATIC ${GAME_SOURCES})
add_dependencies(game base)
target_link_libraries(game PUBLIC base)
set_target_properties(game PROPERTIES OUTPUT_NAME "VolcanoGame")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GAME_SOURCES})

target_include_directories(game
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

configure_file(
    include/Volcano/Game/Config.hpp.in
    include/Volcano/Game/Config.hpp
    )

find_package(Qt6 COMPONENTS Gui REQUIRED)
target_link_libraries(game PUBLIC Qt6::Gui)

include(FindBullet)
if(BULLET_FOUND)
    target_include_directories(game PUBLIC ${BULLET_INCLUDE_DIRS})
    target_link_libraries(game PUBLIC ${BULLET_LIBRARIES})
else()
    message(FATAL_ERROR "Failed to find BulletPhysics.")
endif()

