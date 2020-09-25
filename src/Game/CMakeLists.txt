
target_sources(Volcano
    PRIVATE
        Camera.cpp
        Component.cpp
        DirectionalLight.cpp
        Entity.cpp
        Light.cpp
        Model.cpp
        OrthoCamera.cpp
        PerpectiveCamera.cpp
        PointLight.cpp
        SpotLight.cpp
        VM.cpp
        World.cpp
    )

add_subdirectory(Client Client)
add_subdirectory(Server Server)
