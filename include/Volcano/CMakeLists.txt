
target_sources(Volcano
	PUBLIC
		Camera.hpp
		Common.hpp
		Component.hpp
		DirectionalLight.hpp
		Entity.hpp
		Light.hpp
		Model.hpp
		OrthoCamera.hpp
		PerpectiveCamera.hpp
		PointLight.hpp
		Resource.hpp
		Snapshot.hpp
		SpotLight.hpp
        View.hpp
		World.hpp
    PRIVATE
        Config.hpp.in
	)

configure_file(
    Config.hpp.in
    Config.hpp
    )

add_subdirectory(OpenGL OpenGL)
