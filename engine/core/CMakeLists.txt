
file(GLOB_RECURSE VOLCANO_CORE_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

configure_file(
    include/Volcano/Config.hpp.in
    include/Volcano/Config.hpp
    )

add_library(core STATIC ${VOLCANO_CORE_SOURCES})
set_target_properties(core PROPERTIES OUTPUT_NAME "VolcanoCore")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VOLCANO_CORE_SOURCES})

target_include_directories(core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

find_package(Qt6 REQUIRED Gui Qml)
target_link_libraries(core PUBLIC Qt6::Gui Qt6::Qml)

include(FindBullet)
if(BULLET_FOUND)
    target_include_directories(core PUBLIC ${BULLET_INCLUDE_DIRS})
    target_link_libraries(core PUBLIC ${BULLET_LIBRARIES})
else()
    message(FATAL_ERROR "Failed to find BulletPhysics.")
endif()
