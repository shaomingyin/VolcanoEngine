
find_package(QtCreator COMPONENTS Core REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

set(VOLCANO_EDITOR_SOURCES
    include/Volcano/Editor/Common.h
    include/Volcano/Editor/Config.h.in
    include/Volcano/Editor/Plugin.h
    include/Volcano/Editor/Plugin.json.in
    include/Volcano/Editor/Project.h
    include/Volcano/Editor/ProjectView.h
    include/Volcano/Editor/ProjectWizard.h
    include/Volcano/Editor/SceneEditor.h
    include/Volcano/Editor/SceneObjectWizard.h
    include/Volcano/Editor/SceneWizard.h
    src/Volcano/Editor/Plugin.cpp
    src/Volcano/Editor/Project.cpp
    src/Volcano/Editor/ProjectView.cpp
    src/Volcano/Editor/ProjectWizard.cpp
    src/Volcano/Editor/SceneEditor.cpp
    src/Volcano/Editor/SceneObjectWizard.cpp
    src/Volcano/Editor/SceneWizard.cpp
)

configure_file(
    include/Volcano/Editor/Config.h.in
    include/Volcano/Editor/Config.h
)

configure_file(
    include/Volcano/Editor/Plugin.json.in
    include/Volcano/Editor/Plugin.json
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
