
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
		Resource.cpp
		SpotLight.cpp
        World.cpp
	)

add_subdirectory(OpenGL OpenGL)
