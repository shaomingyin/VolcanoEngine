
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

volcano_option(VOLCANO_GUI "Enable GUI." ON)
if(VOLCANO_GUI)
	add_subdirectory(Gui)
endif()

volcano_option(VOLCANO_NETWORK "Enable network." ON)
if(VOLCANO_NETWORK)
	add_subdirectory(Network)
endif()

volcano_option(VOLCANO_PLAYER "Enable player." VOLCANO_ACOUSTICS AND VOLCANO_GRAPHICS AND VOLCANO_PHYSICS AND VOLCANO_GUI)
if(VOLCANO_PLAYER)
	add_subdirectory(Player)
endif()
