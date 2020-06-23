
add_library(VolcanoEditor SHARED
    Common.cpp
    Common.h
    Plugin.cpp
    Plugin.h
    SceneEditor.cpp
    SceneEditor.h
    SceneEditorFactory.cpp
    SceneEditorFactory.h
    Config.h.in
    Plugin.json.in
	)

add_dependencies(VolcanoEditor
    Volcano
    )

target_link_libraries(VolcanoEditor
    Volcano
    Qt5::Core
    Qt5::Gui
    Qt5::Concurrent
    Qt5::Qml
    )

configure_file(
    Config.h.in
    Config.h
    )

configure_file(
    Plugin.json.in
    Plugin.json
    )
