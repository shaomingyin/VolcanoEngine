
target_sources(Volcano
    PUBLIC
        Common.hpp
        Camera.hpp
        Config.hpp.in
        DirectionalLight.hpp
        Entity.hpp
        Light.hpp
        Model.hpp
        Node.hpp
        OpenGL.hpp
        OrthoCamera.hpp
        PerpectiveCamera.hpp
        PointLight.hpp
        Resource.hpp
        Scene.hpp
        SpotLight.hpp
        Viewport.hpp
        World.hpp
        WorldAmbient.hpp
    )

configure_file(
    Config.hpp.in
    Config.hpp
    )

add_subdirectory(OpenGL OpenGL)
