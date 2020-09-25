cmake_minimum_required(VERSION 3.13)

project(VolcanoEngine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH "${CMAKE_HOME_DIRECTORY}/cmake")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

# Qt

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5QuickControls2 REQUIRED)

link_libraries(
    Qt5::Core
    Qt5::Concurrent
    Qt5::Network
    Qt5::Gui
    Qt5::Qml
    Qt5::QuickControls2
    )

# BulletPhysics

include(FindBullet)
if(BULLET_FOUND)
    include_directories(${BULLET_INCLUDE_DIRS})
    link_libraries(${BULLET_LIBRARIES})
else()
    message(FATAL_ERROR "BulletPHysics not found.")
endif()

# Assimp

include(FindAssimp)
if(ASSIMP_FOUND)
    include_directories(${ASSIMP_FOUND})
    link_libraries(${ASSIMP_LIBRARIES})
else()
    message(FATAL_ERROR "Assimp not found.")
endif()

set(VOLCANO_DEBUG ON CACHE BOOL "Enable debug functions")

if(VOLCANO_DEBUG)
    add_definitions(-DQT_QML_DEBUG)
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
    )

add_library(Volcano SHARED)

add_subdirectory(include include)
add_subdirectory(src src)

get_target_property(VOLCANO_SOURCES Volcano SOURCES)
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${VOLCANO_SOURCES})

target_compile_definitions(Volcano PRIVATE VOLCANO)

add_subdirectory(test test)
add_subdirectory(tools tools)
add_subdirectory(docs docs)
