
find_package(Qt6 COMPONENTS Gui OpenGL Qml Quick REQUIRED)
link_libraries(Qt6::Gui Qt6::OpenGL Qt6::Qml Qt6::Quick)

include(FindAssimp)
if(ASSIMP_FOUND)
    include_directories(${ASSIMP_INCLUDE_DIR})
    link_libraries(${ASSIMP_LIBRARIES})
else()
    message(FATAL_ERROR "Assimp not found.")
endif()

include(FindBullet)
if(BULLET_FOUND)
    include_directories(${BULLET_INCLUDE_DIRS})
    link_libraries(${BULLET_LIBRARIES})
else()
    message(FATAL_ERROR "Failed to find BulletPhysics.")
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

configure_file(
    include/Volcano/Config.hpp.in
    include/Volcano/Config.hpp
    )

file(GLOB_RECURSE ENGINE_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_library(engine STATIC ${ENGINE_SOURCES})
set_target_properties(engine PROPERTIES OUTPUT_NAME "volcano")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_SOURCES})
