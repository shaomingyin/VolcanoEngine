
target_sources(Volcano
    PUBLIC
        Common.hpp
        Camera.hpp
        Config.hpp.in
        DirectionalLight.hpp
        Entity.hpp
        Graphics.hpp
        Light.hpp
        Mesh.hpp
        Node.hpp
        PointLight.hpp
        Resource.hpp
        Scene.hpp
        SpotLight.hpp
        View.hpp
        Viewport.hpp
    )

configure_file(
    Config.hpp.in
    Config.hpp
    )

add_subdirectory(Graphics Graphics)
