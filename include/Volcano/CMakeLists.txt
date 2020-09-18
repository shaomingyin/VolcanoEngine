
target_sources(Volcano
	PUBLIC
        Camera.hpp
        Common.hpp
        Component.hpp
		DirectionalLight.hpp
		Entity.hpp
		Light.hpp
		Material.hpp
		Model.hpp
		Object.hpp
        OrthoCamera.hpp
        PerpectiveCamera.hpp
		Picture.hpp
		PointLight.hpp
		Resource.hpp
		SpotLight.hpp
		World.hpp
	)

add_subdirectory(Details Details)
