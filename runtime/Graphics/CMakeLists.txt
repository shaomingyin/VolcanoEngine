
volcano_add_static_library(Graphics
    OUTPUT_NAME
        VolcanoGraphics
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/GL/glcorearb.h
        include/GL/gl3w.h
        src/gl3w/gl3w.c
		
        include/debug_draw.hpp
        src/debug_draw/debug_draw.cpp
		
		include/Volcano/Graphics/BulletDebugDrawer.h
        include/Volcano/Graphics/Config.h.in
        include/Volcano/Graphics/Common.h
        include/Volcano/Graphics/Mesh.h
        include/Volcano/Graphics/Renderer.h
        include/Volcano/Graphics/Target.h
        include/Volcano/Graphics/View.h
		src/Volcano/Graphics/BulletDebugDrawer.cpp
        src/Volcano/Graphics/Mesh.cpp
        src/Volcano/Graphics/Target.cpp
        src/Volcano/Graphics/Renderer.cpp
        src/Volcano/Graphics/View.cpp
    PUBLIC_LINK_LIBRARIES
        Base
    PUBLIC_LINK_PACKAGES
        assimp assimp::assimp
        Bullet Bullet::Bullet
    PUBLIC_HEADERS
        include/Volcano/Graphics/Common.h
        include/Volcano/Graphics/Renderer.h
        include/Volcano/Graphics/View.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Graphics/Config.h
        DESTINATION include/Volcano/Graphics
)
