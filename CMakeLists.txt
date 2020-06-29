cmake_minimum_required(VERSION 3.13)

project(VolcanoEngine VERSION 0.1.0)

#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5QuickControls2 REQUIRED)

set(VOLCANO_DEBUG ON CACHE BOOL "Enable debug functions")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

add_executable(Volcano)
set_target_properties(Volcano PROPERTIES OUTPUT_NAME "volcano")

target_link_libraries(Volcano
	Qt5::Core
    Qt5::Concurrent
	Qt5::Gui
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickControls2
	)

include(FindBullet)
if(BULLET_FOUND)
    target_include_directories(Volcano PUBLIC ${BULLET_INCLUDE_DIRS})
    target_link_libraries(Volcano ${BULLET_LIBRARIES})
else()
    message(FATAL_ERROR "BulletPhysics not found.")
endif()

add_subdirectory(include include)
add_subdirectory(src src)

get_target_property(VOLCANO_SOURCES Volcano SOURCES)
foreach(SOURCE ${VOLCANO_SOURCES})
    message(">>> ${SOURCE}")
    string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR} "" RELPATH ${SOURCE})
    message(">>> ${RELPATH}")
    list(APPEND VOLCANO_SOURCES_RPATH ${RELPATH})
endforeach()
message(">>> ${VOLCANO_SOURCES_RPATH}")
source_group(TREE "/" FILES ${VOLCANO_SOURCES_RPATH})
source_group(\\mocs REGULAR_EXPRESSION "mocs_.*\\.cpp")

# Others
add_subdirectory(tools tools)
add_subdirectory(docs docs)
