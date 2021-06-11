
find_package(Qt6 REQUIRED Gui OpenGL)

file(GLOB_RECURSE VOLCANO_GRAPHICS_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_library(graphics STATIC ${VOLCANO_GRAPHICS_SOURCES})
set_target_properties(graphics PROPERTIES OUTPUT_NAME "VolcanoGraphics")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VOLCANO_GRAPHICS_SOURCES})

target_include_directories(graphics
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

target_link_libraries(graphics
    PUBLIC
        base
        Qt6::Gui
        Qt6::OpenGL
    )

configure_file(
    include/Volcano/Graphics/Config.hpp.in
    include/Volcano/Graphics/Config.hpp
    )
