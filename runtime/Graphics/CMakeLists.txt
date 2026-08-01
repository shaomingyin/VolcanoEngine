
add_library(Graphics STATIC
    include/GL/gl.hpp
    include/GL/gl3w.h
    include/GL/glcorearb.h
    include/KHR/khrplatform.h
    src/GL/gl3w.c
	
    include/Volcano/Graphics/Camera.h
    include/Volcano/Graphics/Common.h
    include/Volcano/Graphics/Config.h.in
    include/Volcano/Graphics/DirectionalLight.h
    include/Volcano/Graphics/Frame.h
    include/Volcano/Graphics/Light.h
    include/Volcano/Graphics/Material.h
    include/Volcano/Graphics/Mesh.h
    include/Volcano/Graphics/Model.h
    include/Volcano/Graphics/OrthographicCamera.h
    include/Volcano/Graphics/Pass.h
    include/Volcano/Graphics/PerspectiveCamera.h
    include/Volcano/Graphics/PointLight.h
    include/Volcano/Graphics/Renderer.h
    include/Volcano/Graphics/SpotLight.h
    src/Volcano/Graphics/Camera.cpp
    src/Volcano/Graphics/Common.cpp
    src/Volcano/Graphics/DirectionalLight.cpp
    src/Volcano/Graphics/Frame.cpp
    src/Volcano/Graphics/Light.cpp
    src/Volcano/Graphics/Material.cpp
    src/Volcano/Graphics/Mesh.cpp
    src/Volcano/Graphics/Model.cpp
    src/Volcano/Graphics/OrthographicCamera.cpp
    src/Volcano/Graphics/Pass.cpp
    src/Volcano/Graphics/PerspectiveCamera.cpp
    src/Volcano/Graphics/PointLight.cpp
    src/Volcano/Graphics/Renderer.cpp
    src/Volcano/Graphics/SpotLight.cpp
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

target_link_libraries(Graphics PUBLIC World)
