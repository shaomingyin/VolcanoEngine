
add_library(Graphics STATIC
    include/GL/gl.hpp
    include/GL/gl3w.h
    include/GL/glcorearb.h
    include/KHR/khrplatform.h
    src/GL/gl3w.c
	
    include/Volcano/Graphics/Common.h
    include/Volcano/Graphics/Config.h.in
    include/Volcano/Graphics/Material.h
    include/Volcano/Graphics/Mesh.h
    include/Volcano/Graphics/RenderContext.h
    include/Volcano/Graphics/GeometryPass.h
    include/Volcano/Graphics/LightingPass.h
    include/Volcano/Graphics/TransparentPass.h
    include/Volcano/Graphics/Renderer.h
    include/Volcano/Graphics/Target.h
    include/Volcano/Graphics/Texture.h
    src/Volcano/Graphics/Mesh.cpp
    src/Volcano/Graphics/GeometryPass.cpp
    src/Volcano/Graphics/LightingPass.cpp
    src/Volcano/Graphics/TransparentPass.cpp
    src/Volcano/Graphics/Renderer.cpp
    src/Volcano/Graphics/Texture.cpp
)

configure_file(
    include/Volcano/Graphics/Config.h.in
    include/Volcano/Graphics/Config.h
)

target_include_directories(Graphics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(Graphics)
set_target_properties(Graphics PROPERTIES OUTPUT_NAME VolcanoGraphics)

target_link_libraries(Graphics PUBLIC Base Math)

if(BUILD_TESTING)
    add_executable(GraphicsLoaderCheck tests/Loader.cpp)
    target_link_libraries(GraphicsLoaderCheck PRIVATE Graphics GTest::gtest_main)
    gtest_discover_tests(GraphicsLoaderCheck)
endif()
