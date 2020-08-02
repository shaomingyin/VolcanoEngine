
add_executable(VolcanoLauncher
    Main.cpp
    )

add_dependencies(VolcanoLauncher Volcano)
target_link_libraries(VolcanoLauncher Volcano)

get_target_property(VOLCANO_LAUNCHER_SOURCES VolcanoLauncher SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VOLCANO_LAUNCHER_SOURCES})
