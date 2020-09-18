
target_sources(Volcano
	PRIVATE
		Camera.cpp
		Component.cpp
		DirectionalLight.cpp
		Entity.cpp
		Light.cpp
		Material.cpp
		Model.cpp
		Object.cpp
        OrthoCamera.cpp
        PerpectiveCamera.cpp
		Picture.cpp
		PointLight.cpp
		Resource.cpp
		SpotLight.cpp
		World.cpp
	)

add_subdirectory(Details Details)
