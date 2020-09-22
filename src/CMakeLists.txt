
target_sources(Volcano
	PRIVATE
		Camera.cpp
		Component.cpp
		DirectionalLight.cpp
		Entity.cpp
		Light.cpp
        OrthoCamera.cpp
        PerpectiveCamera.cpp
		PointLight.cpp
		Resource.cpp
		Snapshot.cpp
		SpotLight.cpp
        View.cpp
		World.cpp
	)

add_subdirectory(OpenGL OpenGL)
