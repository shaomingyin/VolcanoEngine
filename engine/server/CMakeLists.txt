
include_directories(engine
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

link_libraries(game)

file(GLOB_RECURSE SERVER_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_executable(server ${SERVER_SOURCES})
add_dependencies(server game)
set_target_properties(server PROPERTIES OUTPUT_NAME "VolcanoServer")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SERVER_SOURCES})

configure_file(
    include/Volcano/Server/Config.hpp.in
    include/Volcano/Server/Config.hpp
    )
