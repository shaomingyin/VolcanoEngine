
add_library(Fusion STATIC
    include/Volcano/Fusion/Common.hpp
    include/Volcano/Fusion/Config.hpp.in
    include/Volcano/Fusion/Display.hpp
    include/Volcano/Fusion/ViewBuilder.hpp
    src/Volcano/Fusion/Display.cpp
    src/Volcano/Fusion/ViewBuilder.cpp
)

configure_file(
    include/Volcano/Fusion/Config.hpp.in
    include/Volcano/Fusion/Config.hpp
)

target_include_directories(Fusion PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(Fusion)
set_target_properties(Fusion PROPERTIES OUTPUT_NAME VolcanoFusion)

target_link_libraries(Fusion PUBLIC World Graphics Gui)
