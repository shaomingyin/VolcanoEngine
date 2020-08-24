
target_sources(Volcano
	PRIVATE
		Camera.cpp
		Core.cpp
		DirectionalLight.cpp
		Entity.cpp
		Light.cpp
		Node.cpp
        Model.cpp
        OrthoCamera.cpp
        PerpectiveCamera.cpp
		PointLight.cpp
		Resource.cpp
		Scene.cpp
		SpotLight.cpp
		Viewport.cpp
        World.cpp
        WorldAmbient.cpp
	)

add_subdirectory(OpenGL OpenGL)
