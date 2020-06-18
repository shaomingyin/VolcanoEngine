
add_subdirectory(BulletPhysics BulletPhysics)
add_subdirectory(OpenGLRenderer OpenGLRenderer)

if(VOLCANO_EDITOR)
    add_subdirectory(Editor Editor)
endif()


target_sources(Kernel
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
