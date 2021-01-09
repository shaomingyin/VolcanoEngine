#
#
#

cmake_minimum_required(VERSION 3.13)

project(VolcanoEngine VERSION 0.1.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_MODULE_PATH "${CMAKE_HOME_DIRECTORY}/cmake")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

add_subdirectory(documents document)
add_subdirectory(engine engine)
add_subdirectory(framework framework)
add_subdirectory(samples samples)
add_subdirectory(test test)
