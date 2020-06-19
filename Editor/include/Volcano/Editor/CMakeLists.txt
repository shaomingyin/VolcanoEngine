
target_sources(Editor
	PUBLIC
		Common.h
        Plugin.h
        SceneEditor.h
        SceneEditorFactory.h
    PRIVATE
        Config.h.in
        Plugin.json.in
	)

configure_file(
    Config.h.in
    Config.h
    )

configure_file(
    Plugin.json.in
    Plugin.json
    )
