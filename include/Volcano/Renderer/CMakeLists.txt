
target_sources(Volcano
    PUBLIC
		Common.h
        Context.h
        Light.h
        Mesh.h
        View.h
    PRIVATE
        Config.h.in
	)

configure_file(
    Config.h.in
    Config.h
    )
