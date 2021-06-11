
find_package(Qt6 REQUIRED Widgets)

file(GLOB_RECURSE VOLCANO_EDITOR_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_executable(editor ${VOLCANO_EDITOR_SOURCES})
set_target_properties(editor PROPERTIES OUTPUT_NAME "VolcanoEditor")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VOLCANO_EDITOR_SOURCES})

target_include_directories(editor
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

target_link_libraries(editor
    PUBLIC
        base
        Qt6::Widgets
    )

configure_file(
    include/Volcano/Editor/Config.hpp.in
    include/Volcano/Editor/Config.hpp
    )
