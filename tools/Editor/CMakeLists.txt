
find_package(QtCreator COMPONENTS Core REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(VOLCANO_EDITOR_SOURCES
    include/Volcano/Editor/AssetManager.h
    include/Volcano/Editor/Common.h
    include/Volcano/Editor/Config.h.in
    include/Volcano/Editor/GeneralSettings.h
    include/Volcano/Editor/KitAspect.h
    include/Volcano/Editor/KitAspectFactory.h
    include/Volcano/Editor/KitSettings.h
    include/Volcano/Editor/Plugin.h
    include/Volcano/Editor/PropertyView.h
    include/Volcano/Editor/SceneDocument.h
    include/Volcano/Editor/SceneEditor.h
    include/Volcano/Editor/SceneEditorFactory.h
    include/Volcano/Editor/SceneEditorToolBar.h
    include/Volcano/Editor/SceneEditorViewport.h
    include/Volcano/Editor/SceneTree.h
    include/Volcano/Editor/SdkSettings.h
    src/Volcano/Editor/AssetManager.cpp
    src/Volcano/Editor/GeneralSettings.cpp
    src/Volcano/Editor/KitAspect.cpp
    src/Volcano/Editor/KitAspectFactory.cpp
    src/Volcano/Editor/KitSettings.cpp
    src/Volcano/Editor/Plugin.cpp
    src/Volcano/Editor/PropertyView.cpp
    src/Volcano/Editor/SceneDocument.cpp
    src/Volcano/Editor/SceneEditor.cpp
    src/Volcano/Editor/SceneEditorFactory.cpp
    src/Volcano/Editor/SceneEditorToolBar.cpp
    src/Volcano/Editor/SceneEditorViewport.cpp
    src/Volcano/Editor/SceneTree.cpp
    src/Volcano/Editor/SdkSettings.cpp
    Editor.json.in
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
    PLUGIN_DEPENDS
        QtCreator::CMakeProjectManager
        QtCreator::ProjectExplorer
        QtCreator::QtSupport
        QtCreator::GLSLEditor
        QtCreator::Core
    DEPENDS
        World
		Graphics
    SOURCES
        ${VOLCANO_EDITOR_SOURCES}
)

find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)
target_link_libraries(Editor PUBLIC Qt6::OpenGLWidgets)

qt_add_resources(Editor res
    PREFIX "/volcano/editor" FILES
        templates/wizards/project/wizard.json
        templates/wizards/project/CMakeLists.txt
        templates/wizards/project/Resources.qrc
        templates/wizards/project/Assets/CMakeLists.txt
        templates/wizards/project/Modules/CMakeLists.txt
        templates/wizards/project/World/CMakeLists.txt
        templates/wizards/project/World/Startup.volcano-scene
        templates/wizards/scene/wizard.json
)

volcano_source_tree(${VOLCANO_EDITOR_SOURCES})
volcano_set_target_folder(Editor)
set_target_properties(Editor PROPERTIES OUTPUT_NAME "VolcanoEditor")
