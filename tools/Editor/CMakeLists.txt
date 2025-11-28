
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
    include/Volcano/Editor/Context.h
    include/Volcano/Editor/GeneralSettings.h
    include/Volcano/Editor/KitAspect.h
    include/Volcano/Editor/KitAspectFactory.h
    include/Volcano/Editor/Plugin.h
    include/Volcano/Editor/PropertyView.h
    include/Volcano/Editor/Qmlizer.h
    include/Volcano/Editor/Sdk.h
    include/Volcano/Editor/SdkManager.h
    include/Volcano/Editor/SdkSettings.h
    include/Volcano/Editor/WorldDocument.h
    include/Volcano/Editor/WorldEditor.h
    include/Volcano/Editor/WorldEditorFactory.h
    include/Volcano/Editor/WorldEditorToolBar.h
    include/Volcano/Editor/WorldEditorViewport.h
    include/Volcano/Editor/WorldTree.h
    src/Volcano/Editor/AssetManager.cpp
    src/Volcano/Editor/Context.cpp
    src/Volcano/Editor/GeneralSettings.cpp
    src/Volcano/Editor/KitAspect.cpp
    src/Volcano/Editor/KitAspectFactory.cpp
    src/Volcano/Editor/Plugin.cpp
    src/Volcano/Editor/PropertyView.cpp
    src/Volcano/Editor/Qmlizer.cpp
    src/Volcano/Editor/Sdk.cpp
    src/Volcano/Editor/SdkManager.cpp
    src/Volcano/Editor/SdkSettings.cpp
    src/Volcano/Editor/WorldDocument.cpp
    src/Volcano/Editor/WorldEditor.cpp
    src/Volcano/Editor/WorldEditorFactory.cpp
    src/Volcano/Editor/WorldEditorToolBar.cpp
    src/Volcano/Editor/WorldEditorViewport.cpp
    src/Volcano/Editor/WorldTree.cpp
    Editor.json.in
)

set(VOLCANO_EDITOR_MIMETYPE_PROJECT_FILE "application/volcano-project-file")
set(VOLCANO_EDITOR_MIMETYPE_WORLD_FILE "application/volcano-scene-file")

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
        Game
        Graphics
    SOURCES
        ${VOLCANO_EDITOR_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VOLCANO_EDITOR_SOURCES})

set_target_properties(Editor PROPERTIES OUTPUT_NAME "VolcanoEditor")

qt_add_resources(Editor res PREFIX "/volcano/editor" FILES
    templates/wizards/project/wizard.json
    templates/wizards/project/CMakeLists.txt
    templates/wizards/project/Resources.qrc
    templates/wizards/project/Assets/CMakeLists.txt
    templates/wizards/project/Modules/CMakeLists.txt
    templates/wizards/project/World/CMakeLists.txt
    templates/wizards/project/World/Startup.volcano-scene
    templates/wizards/scene/wizard.json
)

volcano_link_qt(Editor OpenGLWidgets)
