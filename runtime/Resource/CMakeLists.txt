
volcano_add_static_library(Resource
    OUTPUT_NAME
        VolcanoResource
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Resource/Common.h
		include/Volcano/Resource/Map.h
        include/Volcano/Resource/Material.h
        include/Volcano/Resource/Mesh.h
		include/Volcano/Resource/Model.h
        include/Volcano/Resource/Texture.h
        src/Volcano/Resource/Common.cpp
		src/Volcano/Resource/Map.cpp
        src/Volcano/Resource/Material.cpp
        src/Volcano/Resource/Mesh.cpp
		src/Volcano/Resource/Model.cpp
        src/Volcano/Resource/Texture.cpp
    PUBLIC_LINK_LIBRARIES
        Base
    PUBLIC_QT_MODULES
        Gui
    PUBLIC_LINK_PACKAGES
        assimp assimp::assimp
)
