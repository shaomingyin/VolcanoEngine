
file(GLOB_RECURSE VOLCANO_LAUNCHER_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_library(launcher STATIC ${VOLCANO_LAUNCHER_SOURCES})
set_target_properties(launcher PROPERTIES OUTPUT_NAME "VolcanoLauncher")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VOLCANO_LAUNCHER_SOURCES})

target_include_directories(launcher
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

target_link_libraries(launcher
    PRIVATE
        game
        ui
        base
    )

configure_file(
    include/Volcano/Launcher/Config.hpp.in
    include/Volcano/Launcher/Config.hpp
    )
