#
#
#

cmake_minimum_required(VERSION 3.19)

project(VolcanoEngine VERSION 0.1.0)

if(MSVC)
    add_definitions(-D _CRT_SECURE_NO_WARNINGS)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_MODULE_PATH "${CMAKE_HOME_DIRECTORY}/cmake")

set(VOLCANO_OUT_DIR ${CMAKE_BINARY_DIR}/out)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${VOLCANO_OUT_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${VOLCANO_OUT_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${VOLCANO_OUT_DIR}/bin)

set(VOLCANO_DEBUG ON CACHE BOOL "Enable debug funtions.")

include(CMakeDependentOption)
include(VolcanoUtils)

add_subdirectory(engine engine)
add_subdirectory(framework framework)
add_subdirectory(tools tools)
add_subdirectory(documents documents)
