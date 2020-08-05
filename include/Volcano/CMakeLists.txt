
target_sources(Volcano
    PUBLIC
        Common.hpp

    PRIVATE
        Camera.hpp
        Config.hpp.in
        DirectionalLight.hpp
        Entity.hpp
        Light.hpp
        Mesh.hpp
        Node.hpp
        OpenGLHeap.hpp
        OpenGLMemory.hpp
        OpenGLMesh.hpp
        OpenGLRenderer.hpp
        OpenGLResource.hpp
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
