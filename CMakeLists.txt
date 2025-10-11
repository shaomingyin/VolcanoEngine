
cmake_minimum_required(VERSION 3.24)
project(VolcanoEngine VERSION 0.1.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

set(VOLCANO_DEBUG ON CACHE BOOL "Enable debug functions.")
set(VOLCANO_PROFILE ON CACHE BOOL "Enable profile functions.")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Volcano/Utils)

add_subdirectory(runtime)
add_subdirectory(games)

set(VOLCANO_TOOLS ON CACHE BOOL "Enable tools.")
if(VOLCANO_TOOLS)
	add_subdirectory(tools)
endif()

set(VOLCANO_EXAMPLES OFF CACHE BOOL "Enable examples.")
if(VOLCANO_EXAMPLES)
	add_subdirectory(examples)
endif()

set(VOLCANO_DOCUMENTS ON CACHE BOOL "Enable documents.")
if(VOLCANO_DOCUMENTS)
	add_subdirectory(documents)
endif()
