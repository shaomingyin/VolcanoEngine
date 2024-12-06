
add_subdirectory(Base)
add_subdirectory(World)

volcano_option(VOLCANO_ACOUSTICS "Enable acoustics." ON)
if(VOLCANO_ACOUSTICS)
	add_subdirectory(Acoustics)
endif()

volcano_option(VOLCANO_GRAPHICS "Enable graphics." ON)
if(VOLCANO_GRAPHICS)
	add_subdirectory(Graphics)
endif()

add_subdirectory(System)
