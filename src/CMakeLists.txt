
add_subdirectory(Renderer Renderer)

target_sources(Volcano
    PRIVATE
        Camera.cpp
        Core.cpp
        Entity.cpp
        Node.cpp
        Renderer.cpp
        Resources.qrc
        Scene.cpp
        Viewport.cpp
    )
