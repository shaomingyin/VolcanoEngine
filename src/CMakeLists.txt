
add_subdirectory(Renderer Renderer)

target_sources(Volcano
    PRIVATE
        Camera.cpp
        Editor.cpp
        Entity.cpp
        Launcher.cpp
        Main.cpp
        Node.cpp
        Scene.cpp
        Viewport.cpp
    )
