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

set(VOLCANO_DEBUG ON CACHE BOOL "Enable debug funtions.")
set(VOLCANO_OUT_DIR "${CMAKE_BINARY_DIR}/out" CACHE PATH "Build output directory.")

add_subdirectory(engine engine)
add_subdirectory(documents documents)
