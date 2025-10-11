
add_library(Graphics STATIC
    include/Volcano/Graphics/Common.h
    include/Volcano/Graphics/Config.h.in
    include/Volcano/Graphics/Renderer.h
    src/Volcano/Graphics/Common.cpp
    src/Volcano/Graphics/Renderer.cpp
)

configure_file(
    include/Volcano/Graphics/Config.h.in
    include/Volcano/Graphics/Config.h
)

target_include_directories(Graphics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

set_target_properties(Graphics PROPERTIES OUTPUT_NAME VolcanoGraphics)

target_link_libraries(Graphics Base)

find_package(SFML REQUIRED COMPONENTS sfml-graphics)
target_link_libraries(Base sfml-graphics)
