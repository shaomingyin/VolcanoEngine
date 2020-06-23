
target_sources(Volcano
    PUBLIC
        Camera.h
        Common.h
        Light.h
        Mesh.h
    PRIVATE
        Config.h.in
	)

configure_file(
    Config.h.in
    Config.h
    )
