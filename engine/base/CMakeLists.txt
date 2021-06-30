
file(GLOB_RECURSE BASE_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_library(base STATIC ${BASE_SOURCES})
set_target_properties(base PROPERTIES OUTPUT_NAME "VolcanoBase")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${BASE_SOURCES})

target_compile_definitions(base
    PUBLIC
        _LARGEFILE_SOURCE
        _LARGEFILE64_SOURCE
        _FILE_OFFSET_BITS=64
    )

target_include_directories(base
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

configure_file(
    include/Volcano/Config.hpp.in
    include/Volcano/Config.hpp
    )

include(FindPhysFS)
if(PHYSFS_FOUND)
    target_include_directories(base PUBLIC ${PHYSFS_INCLUDE_DIR})
    target_link_libraries(base PUBLIC ${PHYSFS_LIBRARY})
else()
    message(FATAL_ERROR "PhysFS not found.")
endif()

