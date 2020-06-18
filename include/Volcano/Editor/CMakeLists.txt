
target_sources(Editor
	PRIVATE
		Common.h
        Plugin.h
        Plugin.json.in
        SceneEditor.h
        SceneEditorFactory.h
	)

configure_file(
    Plugin.json.in
    Plugin.json
    )
