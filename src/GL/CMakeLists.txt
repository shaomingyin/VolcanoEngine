
include(FindOpenGL)
if(OPENGL_FOUND)
    target_include_directories(Volcano PUBLIC ${OPENGL_INCLUDE_DIR})
    target_link_libraries(Volcano ${OPENGL_LIBRARIES})
else()
    message(FATAL_ERROR "OpenGL not found.")
endif()

target_sources(Volcano
    PRIVATE
        Buffer.cpp
        Common.cpp
        FrameBuffer.cpp
        gl3w.c
        Program.cpp
        Shader.cpp
        Texture.cpp
        VertexArray.cpp
    )
