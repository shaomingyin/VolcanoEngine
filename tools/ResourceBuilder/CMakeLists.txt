
volcano_add_executable(ResourceBuilder
    OUTPUT_NAME
        VolcanoResourceBuilder
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/ResourceBuilder/Common.h
		include/Volcano/ResourceBuilder/Linker.h
        include/Volcano/ResourceBuilder/ModelCompiler.h
		include/Volcano/ResourceBuilder/Tool.h
		src/Volcano/ResourceBuilder/Linker.cpp
        src/Volcano/ResourceBuilder/Main.cpp
        src/Volcano/ResourceBuilder/ModelCompiler.cpp
		src/Volcano/ResourceBuilder/Tool.cpp
    PUBLIC_DEFINITIONS
        SDL_MAIN_HANDLED
    PUBLIC_LINK_LIBRARIES
        Graphics
    PUBLIC_LINK_PACKAGES
		argh argh
        assimp assimp::assimp
)
