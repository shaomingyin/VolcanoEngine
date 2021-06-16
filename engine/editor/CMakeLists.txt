find_package(Qt6 COMPONENTS Widgets REQUIRED)
link_libraries(Qt6::Widgets)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

configure_file(
    include/Volcano/Editor/Config.hpp.in
    include/Volcano/Editor/Config.hpp
    )

file(GLOB_RECURSE EDITOR_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_executable(editor ${EDITOR_SOURCES})
set_target_properties(editor PROPERTIES OUTPUT_NAME "VolcanoEditor")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${EDITOR_SOURCES})
add_dependencies(editor launcher)
target_link_libraries(editor PUBLIC runtime)
