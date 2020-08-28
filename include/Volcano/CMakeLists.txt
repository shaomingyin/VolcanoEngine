
target_sources(Volcano
    PUBLIC
        Common.hpp
        Component.hpp
        Camera.hpp
        Config.hpp.in
        DirectionalLight.hpp
        Entity.hpp
        Light.hpp
        Model.hpp
        OpenGL.hpp
        OrthoCamera.hpp
        PerpectiveCamera.hpp
        PointLight.hpp
        Resource.hpp
        SpotLight.hpp
        World.hpp
    )

configure_file(
    Config.hpp.in
    Config.hpp
    )

add_subdirectory(OpenGL OpenGL)
