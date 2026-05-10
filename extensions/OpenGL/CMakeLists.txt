
add_library(OpenGL STATIC
    include/GL/gl3w.h
    include/GL/glcorearb.h
    include/KHR/khrplatform.h
    src/GL/gl3w.c

    include/Volcano/OpenGL/Common.h
    include/Volcano/OpenGL/Config.h.in
    include/Volcano/OpenGL/Renderer.h
    src/Volcano/OpenGL/Renderer.cpp
)

configure_file(
    include/Volcano/OpenGL/Config.h.in
    include/Volcano/OpenGL/Config.h
)

target_include_directories(OpenGL PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(OpenGL)
set_target_properties(OpenGL PROPERTIES OUTPUT_NAME VolcanoOpenGL)

target_link_libraries(OpenGL PUBLIC Graphics)
