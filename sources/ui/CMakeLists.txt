
include(FindOpenGL)
if(NOT OPENGL_FOUND)
    message(FATAL_ERROR "OpenGL not found.")
endif()

set(GL_SOURCES
    include/GL/vgl.h
    include/GL/glcorearb.h
    include/KHR/khrplatform.h
    src/GL/vgl.c

    include/GL/Buffer.hpp
    include/GL/Common.hpp
    include/GL/FrameBuffer.hpp
    include/GL/Object.hpp
    include/GL/Program.hpp
    include/GL/Shader.hpp
    include/GL/Texture.hpp
    include/GL/VertexArray.hpp
    src/GL/Buffer.cpp
    src/GL/FrameBuffer.cpp
    src/GL/Object.cpp
    src/GL/Program.cpp
    src/GL/Shader.cpp
    src/GL/Texture.cpp
    src/GL/VertexArray.cpp
    )

set(NANOVG_SOURCES
    include/fontstash.h
    include/nanovg.h
    include/nanovg_gl.h
    include/nanovg_gl_utils.h
    include/stb_image.h
    include/stb_truetype.h
    src/nanovg/nanovg.c
    )

set(OUI_SOURCES
    include/oui.h
    )

volcano_library(ui
    OUTPUT_NAME
        UI
    DEPENDENCIES
        game
        base
    PUBLIC_LINK_LIBRARIES
        game
        base
        ${OPENGL_LIBRARIES}
    PUBLIC_INCLUDE_DIRECTORIES
        ${OPENGL_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        ${GL_SOURCES}
        ${NANOVG_SOURCES}
        ${OUI_SOURCES}
        include/Volcano/UI/Common.hpp
        include/Volcano/UI/Graphics/Buffer.hpp
        include/Volcano/UI/Graphics/Common.hpp
        include/Volcano/UI/Graphics/DirectionalLight.hpp
        include/Volcano/UI/Graphics/Heap.hpp
        include/Volcano/UI/Graphics/Light.hpp
        include/Volcano/UI/Graphics/Material.hpp
        include/Volcano/UI/Graphics/Memory.hpp
        include/Volcano/UI/Graphics/Mesh.hpp
        include/Volcano/UI/Graphics/PointLight.hpp
        include/Volcano/UI/Graphics/Renderer.hpp
        include/Volcano/UI/Graphics/SpotLight.hpp
        include/Volcano/UI/Graphics/View.hpp
        src/Volcano/UI/Graphics/Buffer.cpp
        src/Volcano/UI/Graphics/DirectionalLight.cpp
        src/Volcano/UI/Graphics/Heap.cpp
        src/Volcano/UI/Graphics/Light.cpp
        src/Volcano/UI/Graphics/Material.cpp
        src/Volcano/UI/Graphics/Memory.cpp
        src/Volcano/UI/Graphics/Mesh.cpp
        src/Volcano/UI/Graphics/Renderer.cpp
        src/Volcano/UI/Graphics/SpotLight.cpp
        src/Volcano/UI/Input/Common.cpp
        src/Volcano/UI/Sound/Common.cpp
    CONFIGURE_FILES
        include/Volcano/UI/Config.hpp.in
    )
