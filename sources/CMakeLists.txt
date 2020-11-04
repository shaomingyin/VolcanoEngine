
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5QuickControls2 REQUIRED)

link_libraries(
    Qt5::Core
    Qt5::Concurrent
    Qt5::Network
    Qt5::Gui
    Qt5::Widgets
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

# Threads

include(FindThreads)
if(Threads_FOUND)
    if (CMAKE_THREAD_LIBS_INIT)
        link_libraries(${CMAKE_THREAD_LIBS_INIT})
    endif()
else()
    message(FATAL_ERROR "Threads not found.")
endif()

# Volcano

set(VOLCANO_DEBUG ON CACHE BOOL "Enable debug functions")

if(VOLCANO_DEBUG)
    add_definitions(-DQT_QML_DEBUG)
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

file(GLOB_RECURSE VOLCANO_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.hpp")
file(GLOB_RECURSE VOLCANO_CONFIGS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "Config.hpp.in")
file(GLOB_RECURSE VOLCANO_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")

set(VOLCANO_ALL_SOURCES ${VOLCANO_HEADERS} ${VOLCANO_CONFIGS} ${VOLCANO_SOURCES})

add_executable(Volcano ${VOLCANO_ALL_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VOLCANO_ALL_SOURCES})

foreach(CONFIG ${VOLCANO_CONFIGS})
    string(REGEX REPLACE "\\.hpp\\.in$" ".hpp" CONFIG_OUTPUT ${CONFIG})
    message(STATUS "Configuring ${CONFIG} to ${CONFIG_OUTPUT}...")
    configure_file(${CONFIG} ${CONFIG_OUTPUT})
endforeach()
