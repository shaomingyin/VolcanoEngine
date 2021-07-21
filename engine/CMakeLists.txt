
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include(FindPhysFS)
if(PHYSFS_FOUND)
    include_directories(${PHYSFS_INCLUDE_DIR})
    link_libraries(${PHYSFS_LIBRARY})
else()
    message(FATAL_ERROR "PhysFS not found.")
endif()

include(FindBullet)
if(BULLET_FOUND)
    include_directories(${BULLET_INCLUDE_DIRS})
    link_libraries(${BULLET_LIBRARIES})
else()
    message(FATAL_ERROR "Bullet not found.")
endif()

find_package(Qt6 COMPONENTS Core Concurrent Gui Qml Quick REQUIRED)
link_libraries(Qt6::Core Qt6::Concurrent Qt6::Gui Qt6::Qml Qt6::Quick)

add_definitions(
    -D_LARGEFILE_SOURCE
    -D_LARGEFILE64_SOURCE
    -D_FILE_OFFSET_BITS=64
    )

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

file(GLOB_RECURSE ENGINE_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_executable(engine ${ENGINE_SOURCES})
set_target_properties(engine PROPERTIES OUTPUT_NAME "VolcanoEngine")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_SOURCES})

configure_file(
    include/Volcano/Config.hpp.in
    include/Volcano/Config.hpp
    )
