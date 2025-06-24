
find_package(QtCreator COMPONENTS Core REQUIRED)

set(VOLCANO_EDITOR_PROJECT_MIMETYPE "application/x-volcano-project")
set(VOLCANO_EDITOR_SCENE_MIMETYPE "application/x-volcano-scene")

set(VOLCANO_EDITOR_SOURCES
    include/Volcano/Editor/BuildConfiguration.h
    include/Volcano/Editor/BuildConfigurationFactory.h
    include/Volcano/Editor/BuildSystem.h
    include/Volcano/Editor/Common.h
    include/Volcano/Editor/Config.h.in
    include/Volcano/Editor/EntityNode.h
    include/Volcano/Editor/LightNode.h
    include/Volcano/Editor/Plugin.h
    include/Volcano/Editor/Project.h
    include/Volcano/Editor/ProjectView.h
    include/Volcano/Editor/ProjectWizard.h
    include/Volcano/Editor/ProjectWizardFactory.h
    include/Volcano/Editor/SceneEditor.h
    include/Volcano/Editor/SceneNode.h
    include/Volcano/Editor/SceneObjectWizard.h
    include/Volcano/Editor/SceneObjectWizardFactory.h
    include/Volcano/Editor/SceneWizard.h
    include/Volcano/Editor/SceneWizardFactory.h
    include/Volcano/Editor/WorldNode.h
    src/Volcano/Editor/BuildConfiguration.cpp
    src/Volcano/Editor/BuildConfigurationFactory.cpp
    src/Volcano/Editor/BuildSystem.cpp
    src/Volcano/Editor/EntityNode.cpp
    src/Volcano/Editor/LightNode.cpp
    src/Volcano/Editor/Plugin.cpp
    src/Volcano/Editor/Project.cpp
    src/Volcano/Editor/ProjectView.cpp
    src/Volcano/Editor/ProjectWizard.cpp
    src/Volcano/Editor/ProjectWizardFactory.cpp
    src/Volcano/Editor/SceneEditor.cpp
    src/Volcano/Editor/SceneNode.cpp
    src/Volcano/Editor/SceneObjectWizard.cpp
    src/Volcano/Editor/SceneObjectWizardFactory.cpp
    src/Volcano/Editor/SceneWizard.cpp
    src/Volcano/Editor/SceneWizardFactory.cpp
    src/Volcano/Editor/WorldNode.cpp
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
        QtCreator::QtSupport
        QtCreator::ProjectExplorer
        QtCreator::Core
    DEPENDS
        World
        Graphics
    SOURCES
        ${VOLCANO_EDITOR_SOURCES}
)

volcano_source_tree(${VOLCANO_EDITOR_SOURCES})

target_include_directories(Editor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)
