
volcano_add_shared_library(OpenGL
    OUTPUT_NAME
        VolcanoGraphicsOpenGL
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Graphics/OpenGL/Config.h.in
        include/Volcano/Graphics/OpenGL/Common.h
        include/Volcano/Graphics/OpenGL/Driver.h
        src/Volcano/Graphics/OpenGL/Driver.cpp
        include/GL/gl3w.h
        include/GL/glcorearb.h
        src/gl3w/gl3w.c
    PUBLIC_LINK_LIBRARIES
        Graphics
    PUBLIC_HEADERS
        include/Volcano/Graphics/OpenGL/Common.h
        include/Volcano/Graphics/OpenGL/Driver.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Graphics/OpenGL/Config.h
        DESTINATION include/Volcano/Graphics/OpenGL
)
