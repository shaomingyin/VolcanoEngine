
add_subdirectory(Renderer Renderer)

target_sources(Volcano
    PUBLIC
		Common.h
        Entity.h
        Object.h
        Renderer.h
        Scene.h
    PRIVATE
        Config.h.in
	)

configure_file(
    Config.h.in
    Config.h
    )
