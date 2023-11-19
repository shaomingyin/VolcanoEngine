#
#
#

cmake_minimum_required(VERSION 3.15)

project(VolcanoEngine VERSION 0.1.0)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_HOME_DIRECTORY}/CMake")

include(CMakeDependentOption)

set(VOLCANO_DEBUG ON CACHE BOOL "Enable debug funtions.")

set(VOLCANO_OUTPUT_DIR "${CMAKE_BINARY_DIR}/Output")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${VOLCANO_OUTPUT_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${VOLCANO_OUTPUT_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${VOLCANO_OUTPUT_DIR}/lib")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include(VolcanoUtils)

find_package(Qt6 COMPONENTS Core)
link_libraries(Qt6::Core)

add_subdirectory(Runtime)
add_subdirectory(Extensions)
add_subdirectory(Framework)
add_subdirectory(Packages)
add_subdirectory(Tools)
add_subdirectory(Documents)
