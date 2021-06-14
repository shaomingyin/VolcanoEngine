
file(GLOB_RECURSE VOLCANO_SERVER_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

configure_file(
    include/Volcano/Server/Config.hpp.in
    include/Volcano/Server/Config.hpp
    )

add_executable(server ${VOLCANO_SERVER_SOURCES})
set_target_properties(server PROPERTIES OUTPUT_NAME "volcano")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VOLCANO_SERVER_SOURCES})
target_link_libraries(server PRIVATE core)
