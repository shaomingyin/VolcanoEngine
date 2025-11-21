
add_library(Utils STATIC
    include/Volcano/Utils/Common.hpp
    include/Volcano/Utils/Config.hpp.in
    include/Volcano/Utils/Display.hpp
    include/Volcano/Utils/ViewBuilder.hpp
    src/Volcano/Utils/Display.cpp
    src/Volcano/Utils/ViewBuilder.cpp
)

configure_file(
    include/Volcano/Utils/Config.hpp.in
    include/Volcano/Utils/Config.hpp
)

target_include_directories(Utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(Utils)
set_target_properties(Utils PROPERTIES OUTPUT_NAME VolcanoUtils)

target_link_libraries(Utils PUBLIC Simulation Graphics Gui)
