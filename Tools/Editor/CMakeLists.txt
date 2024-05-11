
find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(QtCreator COMPONENTS Core REQUIRED)

volcano_glob_sources(EDITOR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${EDITOR_SOURCES})
add_qtc_plugin(Editor
    SKIP_TRANSLATION
    SKIP_PCH
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PLUGIN_DEPENDS
        Game
        QtCreator::Core
    DEPENDS
        QtCreator::ProjectExplorer
        QtCreator::ExtensionSystem
        QtCreator::Utils
        Qt6::Widgets
    SOURCES
        ${EDITOR_SOURCES}
)
