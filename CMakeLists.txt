#
#
#

cmake_minimum_required(VERSION 3.15)

project(VolcanoEngine VERSION 0.1.0)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_HOME_DIRECTORY}/cmake")

include(CMakeDependentOption)

set(VOLCANO_DEBUG ON CACHE BOOL "Enable debug funtions.")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

macro(link_qt_modules)
    foreach(QT_MODULE ${ARGN})
        find_package(Qt6 COMPONENTS ${QT_MODULE} REQUIRED)
        link_libraries(Qt6::${QT_MODULE})
    endforeach()
endmacro()

macro(target_link_qt_modules TARGET SCOPE)
    foreach(QT_MODULE ${ARGN})
        find_package(Qt6 COMPONENTS ${QT_MODULE} REQUIRED)
        target_link_libraries(${TARGET} ${SCOPE} Qt6::${QT_MODULE})
    endforeach()
endmacro()

macro(glob_sources VAR)
    file(GLOB_RECURSE ${VAR} LIST_DIRECTORIES false
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.in
        ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc
        ${CMAKE_CURRENT_SOURCE_DIR}/*.json
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cmake
        ${CMAKE_CURRENT_SOURCE_DIR}/*.vpf
        ${CMAKE_CURRENT_SOURCE_DIR}/*.vwf
        )
endmacro()

add_subdirectory(Runtime Runtime)
add_subdirectory(Compiler Compiler)
add_subdirectory(Linker Linker)
add_subdirectory(Server Server)
add_subdirectory(Launcher Launcher)
add_subdirectory(Editor Editor)
add_subdirectory(Framework Framework)
add_subdirectory(Packages Packages)
