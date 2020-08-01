cmake_minimum_required(VERSION 3.13)

project(VolcanoEngine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5QuickControls2 REQUIRED)

set(VOLCANO_DEBUG ON CACHE BOOL "Enable debug functions")

if(VOLCANO_DEBUG)
    add_definitions(-DQT_QML_DEBUG)
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
    )

add_executable(Volcano)

add_subdirectory(include include)
add_subdirectory(src src)

get_target_property(VOLCANO_SOURCES Volcano SOURCES)
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${VOLCANO_SOURCES})

target_link_libraries(Volcano
	Qt5::Core
	Qt5::Gui
	Qt5::Qml
    Qt5::Quick
	Qt5::QuickControls2
	)

add_subdirectory(docs docs)
