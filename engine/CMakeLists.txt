
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(AUTOMOC ON)
set(AUTORCC ON)
set(AUTOUIC ON)

find_package(Qt6 REQUIRED
    Core
    Gui
    OpenGL
    Widgets
    Qml
    Quick
    )

link_libraries(
    Qt6::Core
    Qt6::OpenGL
    Qt6::Gui
    Qt6::Widgets
    Qt6::Qml
    Qt6::Quick
    )

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

file(GLOB_RECURSE ENGINE_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

configure_file(
    include/Volcano/Config.hpp.in
    include/Volcano/Config.hpp
    )

add_executable(engine ${ENGINE_SOURCES})
set_target_properties(engine PROPERTIES OUTPUT_NAME "volcano")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_SOURCES})
