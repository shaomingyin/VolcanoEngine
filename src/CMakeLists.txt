
target_sources(Volcano
	PRIVATE
		Camera.cpp
		Core.cpp
		DirectionalLight.cpp
		Entity.cpp
        Game.cpp
		Light.cpp
		Node.cpp
        Mesh.cpp
		PointLight.cpp
		Resource.cpp
		Scene.cpp
		SpotLight.cpp
		Viewport.cpp
	)

add_subdirectory(OpenGL OpenGL)
