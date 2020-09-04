
target_sources(Volcano
	PRIVATE
		Module.cpp
		Resource.cpp
		VM.cpp
	)

add_subdirectory(Drivers Drivers)
add_subdirectory(Gui Gui)
add_subdirectory(Scene Scene)
