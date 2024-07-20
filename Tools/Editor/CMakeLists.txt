
set(VOLCANO_EDITOR ON CACHE BOOL "Enable editor.")
if(NOT VOLCANO_EDITOR)
    return()
endif()

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(QtCreator COMPONENTS Core ProjectExplorer REQUIRED)

set(VOLCANO_EDITOR_MIME_PROJECT_FILE "application/volcano.project")
set(VOLCANO_EDITOR_MIME_SCENE_FILE "application/volcano.scene")

configure_file(
    include/Volcano/Editor/Config.hpp.in
    include/Volcano/Editor/Config.hpp
)

configure_file(
    include/Volcano/Editor/Plugin.json.in
    include/Volcano/Editor/Plugin.json
)

volcano_glob_sources(EDITOR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${EDITOR_SOURCES})

add_qtc_plugin(Editor
    PLUGIN_NAME
        VolcanoEditor
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PLUGIN_DEPENDS
        QtCreator::ProjectExplorer
        QtCreator::Core
    DEPENDS
        Game
        QtCreator::Utils
        QtCreator::ExtensionSystem
        Qt6::Widgets
    SOURCES
        ${EDITOR_SOURCES}
)
