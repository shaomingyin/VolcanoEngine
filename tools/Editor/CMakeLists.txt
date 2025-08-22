
find_package(QtCreator COMPONENTS Core REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(VOLCANO_EDITOR_SOURCES
    include/Volcano/Editor/AssetManager.h
    include/Volcano/Editor/BuildConfiguration.h
    include/Volcano/Editor/BuildConfigurationFactory.h
    include/Volcano/Editor/BuildSystem.h
    include/Volcano/Editor/Common.h
    include/Volcano/Editor/Config.h.in
    include/Volcano/Editor/KitAspect.h
    include/Volcano/Editor/KitAspectFactory.h
    include/Volcano/Editor/KitSettingsPage.h
    include/Volcano/Editor/KitSettingsWidget.h
    include/Volcano/Editor/Plugin.h
    include/Volcano/Editor/Project.h
    include/Volcano/Editor/ProjectWizard.h
    include/Volcano/Editor/ProjectWizardFactory.h
    include/Volcano/Editor/PropertyView.h
    include/Volcano/Editor/SceneEditor.h
    include/Volcano/Editor/SceneTree.h
    src/Volcano/Editor/AssetManager.cpp
    src/Volcano/Editor/BuildConfiguration.cpp
    src/Volcano/Editor/BuildConfigurationFactory.cpp
    src/Volcano/Editor/BuildSystem.cpp
    src/Volcano/Editor/KitAspect.cpp
    src/Volcano/Editor/KitAspectFactory.cpp
    src/Volcano/Editor/KitSettingsPage.cpp
    src/Volcano/Editor/KitSettingsWidget.cpp
    src/Volcano/Editor/Plugin.cpp
    src/Volcano/Editor/Project.cpp
    src/Volcano/Editor/ProjectWizard.cpp
    src/Volcano/Editor/ProjectWizardFactory.cpp
    src/Volcano/Editor/PropertyView.cpp
    src/Volcano/Editor/SceneEditor.cpp
    src/Volcano/Editor/SceneTree.cpp
    VolcanoEditor.json.in
)

set(VOLCANO_EDITOR_MIMETYPE_PROJECT_FILE "application/volcano-project-file")
set(VOLCANO_EDITOR_MIMETYPE_SCENE_FILE "application/volcano-scene-file")

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
        QtCreator::CMakeProjectManager
        QtCreator::ProjectExplorer
        QtCreator::QtSupport
        QtCreator::GLSLEditor
        QtCreator::QmlPreview
        QtCreator::Core
    DEPENDS
        World
        Graphics
    SOURCES
        ${VOLCANO_EDITOR_SOURCES}
)

volcano_source_tree(${VOLCANO_EDITOR_SOURCES})
volcano_set_target_folder(Editor)
