
file(GLOB_RECURSE VOLCANO_GRAPHICS_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

configure_file(
    include/Volcano/Graphics/Config.hpp.in
    include/Volcano/Graphics/Config.hpp
    )

add_library(graphics STATIC ${VOLCANO_GRAPHICS_SOURCES})
add_dependencies(graphics core)
target_link_libraries(graphics PUBLIC core)
set_target_properties(graphics PROPERTIES OUTPUT_NAME "VolcanoGraphics")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VOLCANO_GRAPHICS_SOURCES})

target_include_directories(graphics
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

find_package(Qt6 COMPONENTS OpenGL Quick REQUIRED)
target_link_libraries(graphics PUBLIC Qt6::OpenGL Qt6::Quick)
