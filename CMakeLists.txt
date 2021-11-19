#
#
#

cmake_minimum_required(VERSION 3.15)

project(VolcanoEngine VERSION 0.1.0)

if(MSVC)
    add_definitions(-D _CRT_SECURE_NO_WARNINGS)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_MODULE_PATH "${CMAKE_HOME_DIRECTORY}/cmake")

include(CMakeDependentOption)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)

set(VOLCANO_DEBUG ON CACHE BOOL "Enable debug funtions.")
set(VOLCANO_OUT_DIR "${CMAKE_BINARY_DIR}/out" CACHE PATH "Build output directory.")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${VOLCANO_OUT_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${VOLCANO_OUT_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${VOLCANO_OUT_DIR}/lib")

add_subdirectory(Engine Engine)
add_subdirectory(Templates Templates)
add_subdirectory(Packages Packages)
