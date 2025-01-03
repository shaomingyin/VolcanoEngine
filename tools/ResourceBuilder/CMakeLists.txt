
volcano_add_executable(ResourceBuilder
    OUTPUT_NAME
        VolcanoResourceBuilder
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/ResourceBuilder/Common.h
        include/Volcano/ResourceBuilder/GraphicsContext.h
        include/Volcano/ResourceBuilder/MapCompiler.h
        include/Volcano/ResourceBuilder/ModelCompiler.h
		include/Volcano/ResourceBuilder/Tool.h
        include/Volcano/ResourceBuilder/Packager.h
        src/Volcano/ResourceBuilder/GraphicsContext.cpp
        src/Volcano/ResourceBuilder/Main.cpp
        src/Volcano/ResourceBuilder/MapCompiler.cpp
        src/Volcano/ResourceBuilder/ModelCompiler.cpp
		src/Volcano/ResourceBuilder/Tool.cpp
        src/Volcano/ResourceBuilder/Packager.cpp
    PUBLIC_LINK_LIBRARIES
        Graphics
    PUBLIC_LINK_PACKAGES
        assimp assimp::assimp
)
