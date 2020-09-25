
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
        ResourceManager.cpp
		Snapshot.cpp
		SpotLight.cpp
        View.cpp
		World.cpp
	)

add_subdirectory(Graphics Graphics)
