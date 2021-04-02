
# PhysFS
include(FindPhysFS)
if(NOT PHYSFS_FOUND)
    message(FATAL_ERROR "PhysFS not found.")
endif()

file(GLOB_RECURSE BASE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

volcano_static_library(base
    PUBLIC_DEFINITIONS
        NOMINMAX
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
        ${PHYSFS_INCLUDE_DIR}
    PUBLIC_LINK_LIBRARIES
        ${PHYSFS_LIBRARY}
    PRIVATE_SOURCES
        ${BASE_SOURCES}
    )

configure_file(
    include/Volcano/Config.hpp.in
    include/Volcano/Config.hpp
    )
