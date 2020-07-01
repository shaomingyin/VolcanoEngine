cmake_minimum_required(VERSION 3.13)

project(VolcanoEngine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VOLCANO_DEBUG ON CACHE BOOL "Enable debug functions")

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
    )

add_library(Volcano STATIC)
add_subdirectory(include include)
add_subdirectory(src src)

get_target_property(VOLCANO_SOURCES Volcano SOURCES)
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${VOLCANO_SOURCES})

# Others
add_subdirectory(tools tools)
add_subdirectory(docs docs)
