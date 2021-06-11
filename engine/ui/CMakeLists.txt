
find_package(Qt6 REQUIRED Quick)

file(GLOB_RECURSE VOLCANO_UI_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_library(ui STATIC ${VOLCANO_UI_SOURCES})
set_target_properties(ui PROPERTIES OUTPUT_NAME "VolcanoUI")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VOLCANO_UI_SOURCES})

target_include_directories(ui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

target_link_libraries(ui
    PUBLIC
        graphics
        base
        Qt6::Quick
    )

configure_file(
    include/Volcano/UI/Config.hpp.in
    include/Volcano/UI/Config.hpp
    )
