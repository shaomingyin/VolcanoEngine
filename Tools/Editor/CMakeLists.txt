
set(VOLCANO_EDITOR ON CACHE BOOL "Enable editor.")
if(NOT VOLCANO_EDITOR)
    return()
endif()

set(VOLCANO_EDITOR_MIME_PROJECT_FILE "application/volcano.project")
set(VOLCANO_EDITOR_MIME_SCENE_FILE "application/volcano.scene")

configure_file(
    include/Volcano/Editor/Config.hpp.in
    include/Volcano/Editor/Config.hpp
)

volcano_glob_sources(EDITOR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${EDITOR_SOURCES})

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

set(EDITOR_PLUGIN_DEPENDS
    #TextEdior
    ResourceEditor
    ProjectExplorer
    Core
)

set(EDITOR_PLUGIN_RECOMMENDS
    ImageViewer
    QmlProfiler
    QmlPreview
    QmlJSTools
    QmlJSEditor
    BinEditor
    DiffEditor
    GLSLEditor
    CppEditor
)

set(EDITOR_PLUGIN_DEPENDS_JSON_LIST)

foreach(DEP ${EDITOR_PLUGIN_DEPENDS})
    list(APPEND EDITOR_PLUGIN_DEPENDS_PACKAGES "QtCreator::${DEP}")
    list(APPEND EDITOR_PLUGIN_DEPENDS_JSON_LIST "{ \"Name\": \"${DEP}\", \"Version\": \"13.0.1\" }")
endforeach()

list(JOIN EDITOR_PLUGIN_DEPENDS_JSON_LIST ", " EDITOR_PLUGIN_DEPENDS_JSON)
configure_file(
    include/Volcano/Editor/Plugin.json.in
    include/Volcano/Editor/Plugin.json
)
message(STATUS "SMY: ${EDITOR_PLUGIN_DEPENDS_PACKAGES}")
find_package(QtCreator CONFIG REQUIRED COMPONENTS ${EDITOR_PLUGIN_DEPENDS_PACKAGES})
find_package(Qt6 CONFIG REQUIRED COMPONENTS Widgets REQUIRED)

add_qtc_plugin(Editor
    PLUGIN_NAME
        VolcanoEditor
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PLUGIN_DEPENDS
        ${EDITOR_PLUGIN_DEPENDS_PACKAGES}
    DEPENDS
        Game
        QtCreator::ExtensionSystem
        QtCreator::Tasking
        QtCreator::Aggregation
        QtCreator::Utils
        Qt6::Widgets
    SOURCES
        ${EDITOR_SOURCES}
)
