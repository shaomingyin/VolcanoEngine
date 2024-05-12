
set(VOLCANO_EDITOR ON CACHE BOOL "Enable editor.")
if(NOT VOLCANO_EDITOR)
    return()
endif()

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(QtCreator COMPONENTS Core REQUIRED)

volcano_glob_sources(EDITOR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${EDITOR_SOURCES})
configure_file(include/Volcano/Editor/Plugin.json.in include/Volcano/Editor/Plugin.json)
add_qtc_plugin(Editor
    PLUGIN_NAME
        VolcanoEditor
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PLUGIN_DEPENDS
        Game
        QtCreator::ProjectExplorer
        QtCreator::Core
    DEPENDS
        QtCreator::Utils
        QtCreator::ExtensionSystem
        Qt6::Widgets
    SOURCES
        ${EDITOR_SOURCES}
)
#set_target_properties(Editor PROPERTIES OUTPUT_NAME VolcanoEditor)
