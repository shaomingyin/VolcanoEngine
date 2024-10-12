
volcano_add_static_library(Graphics
    OUTPUT_NAME
        VolcanoGraphics
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Graphics/Config.h.in
        include/Volcano/Graphics/Common.h
        include/Volcano/Graphics/Renderer.h
        include/Volcano/Graphics/View.h
        include/GL/glcorearb.h
        include/GL/gl3w.h
        src/Volcano/Graphics/Renderer.cpp
        src/Volcano/Graphics/View.cpp
        src/gl3w/gl3w.c
    PUBLIC_LINK_LIBRARIES
        Base
    PUBLIC_HEADERS
        include/Volcano/Graphics/Common.h
        include/Volcano/Graphics/Renderer.h
        include/Volcano/Graphics/View.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Graphics/Config.h
        DESTINATION include/Volcano/Graphics
)
