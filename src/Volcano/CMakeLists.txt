
add_subdirectory(Graphics Graphics)
add_subdirectory(Physics Physics)
add_subdirectory(Sound Sound)

target_sources(Volcano
	PRIVATE
		Camera.cpp
		Common.cpp
		Entity.cpp
		Event.cpp
		Node.cpp
		Scene.cpp
	)
