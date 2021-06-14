
find_package(Qt6 REQUIRED Gui OpenGL Widgets)

link_libraries(
    Qt6::Gui
    Qt6::OpenGL
    Qt6::Widgets
    )

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

file(GLOB_RECURSE VOLCANO_EDITOR_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

configure_file(
    include/Volcano/Editor/Config.hpp.in
    include/Volcano/Editor/Config.hpp
    )

add_executable(editor ${VOLCANO_EDITOR_SOURCES})
set_target_properties(editor PROPERTIES OUTPUT_NAME "VolcanoEditor")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VOLCANO_EDITOR_SOURCES})
add_dependencies(editor core)
target_link_libraries(editor PRIVATE core)
