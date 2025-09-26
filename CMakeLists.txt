
cmake_minimum_required(VERSION 3.24)
project(VolcanoEngine VERSION 0.1.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOGEN ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

set(VOLCANO_DEBUG ON CACHE BOOL "Enable debug functions.")
set(VOLCANO_PROFILE ON CACHE BOOL "Enable profile functions.")

find_package(Qt6 REQUIRED COMPONENTS Core)
link_libraries(Qt6::Core)

qt_standard_project_setup()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Volcano/Utils)

add_subdirectory(runtime)

volcano_option(VOLCANO_TOOLS "Enable tools." ON)
if(VOLCANO_TOOLS)
	add_subdirectory(tools)
endif()

volcano_option(VOLCANO_EXAMPLES "Enable examples." OFF)
if(VOLCANO_EXAMPLES)
	add_subdirectory(examples)
endif()

volcano_option(VOLCANO_DOCUMENTS "Enable documents." ON)
if(VOLCANO_DOCUMENTS)
	add_subdirectory(documents)
endif()
