
find_package(QtCreator COMPONENTS Core REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

set(VOLCANO_EDITOR_SOURCES
    include/Volcano/Editor/Common.h
    include/Volcano/Editor/Config.h.in
    include/Volcano/Editor/Plugin.h
    src/Volcano/Editor/Plugin.cpp
)

configure_file(
    include/Volcano/Editor/Config.h.in
    include/Volcano/Editor/Config.h
)

add_qtc_plugin(Editor
    OUTPUT_NAME
        VolcanoEditor
    PLUGIN_NAME
        VolcanoEditor
    PLUGIN_DEPENDS
        QtCreator::ProjectExplorer
        QtCreator::Core
    DEPENDS
        ${IDE_PLUGIN_DEPENDENCIES}
        World
        Graphics
    SOURCES
        ${VOLCANO_EDITOR_SOURCES}
)

volcano_source_tree(${VOLCANO_EDITOR_SOURCES})
volcano_set_target_folder(Editor)
