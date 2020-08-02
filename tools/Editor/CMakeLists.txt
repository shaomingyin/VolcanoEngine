
find_package(Qt5Widgets REQUIRED)

add_executable(VolcanoEditor
    Main.cpp
    )

add_dependencies(VolcanoEditor VolcanoLauncher Volcano)

target_link_libraries(VolcanoEditor
    Volcano
    Qt5::Widgets
    )

get_target_property(VOLCANO_EDITOR_SOURCES VolcanoEditor SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VOLCANO_EDITOR_SOURCES})
