
add_subdirectory(Renderer Renderer)

target_sources(Volcano
    PUBLIC
        Camera.h
		Common.h
        Editor.h
        Entity.h
        Launcher.h
        Node.h
        Renderer.h
        Scene.h
        Viewport.h
    PRIVATE
        Config.h.in
	)

configure_file(
    Config.h.in
    Config.h
    )
