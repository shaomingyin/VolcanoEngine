
target_sources(Base
    PUBLIC
        Camera.h
		Common.h
        Entity.h
        Light.h
        Object.h
        Physics.h
        Renderer.h
        Scene.h
    PRIVATE
        Config.h.in
	)

configure_file(
    Config.h.in
    Config.h
    )
