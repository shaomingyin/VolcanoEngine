
add_subdirectory(BulletPhysics BulletPhysics)
add_subdirectory(OpenGLRenderer OpenGLRenderer)

if(VOLCANO_EDITOR)
    add_subdirectory(Editor Editor)
endif()

target_sources(Kernel
    PRIVATE
        Camera.cpp
        Common.cpp
        Entity.cpp
        Light.cpp
        Object.cpp
        Physics.cpp
        Renderer.cpp
        Scene.cpp
    )
