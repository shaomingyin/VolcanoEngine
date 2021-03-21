
add_library(ui STATIC)

add_dependencies(ui
    game
    base
    )

target_link_libraries(ui
    PUBLIC
        game
        base
    )

target_include_directories(ui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

target_sources(ui
    PRIVATE
        include/GL/gl3w.h
        include/GL/glcorearb.h
        include/KHR/khrplatform.h
        src/GL/gl3w.c

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

        include/fontstash.h
        include/nanovg.h
        include/nanovg_gl.h
        include/nanovg_gl_utils.h
        include/stb_image.h
        include/stb_truetype.h
        src/nanovg/nanovg.c

        include/oui.h

        include/Volcano/UI/Common.hpp

        include/Volcano/UI/Graphics/Buffer.hpp
        include/Volcano/UI/Graphics/Common.hpp
        include/Volcano/UI/Config.hpp.in
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

        include/Volcano/UI/Input/Common.hpp
        src/Volcano/UI/Input/Common.cpp

        include/Volcano/UI/Sound/Common.hpp
        src/Volcano/UI/Sound/Common.cpp
    )

configure_file(
    include/Volcano/UI/Config.hpp.in
    include/Volcano/UI/Config.hpp
    )

# OpenGL
include(FindOpenGL)
if(OPENGL_FOUND)
    target_include_directories(ui PUBLIC ${OPENGL_INCLUDE_DIR})
    target_link_libraries(ui PUBLIC ${OPENGL_LIBRARIES})
else()
    message(FATAL_ERROR "OpenGL not found.")
endif()

volcano_target_post_setup(ui)

