
file(GLOB_RECURSE VOLCANO_SERVER_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_executable(server ${VOLCANO_SERVER_SOURCES})
set_target_properties(server PROPERTIES OUTPUT_NAME "VolcanoServer")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VOLCANO_SERVER_SOURCES})

include_directories(server
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

target_link_libraries(server
    PUBLIC
        base
    )

configure_file(
    include/Volcano/Server/Config.hpp.in
    include/Volcano/Server/Config.hpp
    )
