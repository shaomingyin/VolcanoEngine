
add_library(Graphics STATIC
    include/GL/gl3w.h
    include/GL/glcorearb.h
    include/KHR/khrplatform.h
    src/GL/gl3w.c
    include/Volcano/Graphics/Common.hpp
    include/Volcano/Graphics/Config.hpp.in
    include/Volcano/Graphics/Context.hpp
    include/Volcano/Graphics/Renderer.hpp
    include/Volcano/Graphics/View.hpp
    include/Volcano/Graphics/VisibleSet.hpp
    src/Volcano/Graphics/Common.cpp
    src/Volcano/Graphics/Context.cpp
    src/Volcano/Graphics/Renderer.cpp
    src/Volcano/Graphics/View.cpp
    src/Volcano/Graphics/VisibleSet.cpp
)

configure_file(
    include/Volcano/Graphics/Config.hpp.in
    include/Volcano/Graphics/Config.hpp
)

target_include_directories(Graphics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(Graphics)
set_target_properties(Graphics PROPERTIES OUTPUT_NAME VolcanoGraphics)

target_link_libraries(Graphics PUBLIC Base)

find_package(SFML REQUIRED COMPONENTS sfml-graphics)
target_link_libraries(Graphics PUBLIC sfml-graphics)
