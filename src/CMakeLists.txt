
target_sources(Volcano
	PRIVATE
		Camera.cpp
		Core.cpp
		DirectionalLight.cpp
		Entity.cpp
		Light.cpp
		Node.cpp
        Mesh.cpp
		PointLight.cpp
		Resource.cpp
		Scene.cpp
		SpotLight.cpp
		View.cpp
		Viewport.cpp
	)

add_subdirectory(Graphics Graphics)
