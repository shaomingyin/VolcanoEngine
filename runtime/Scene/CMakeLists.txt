
add_library(Scene STATIC
    include/Volcano/Scene/Common.hpp
    include/Volcano/Scene/Config.hpp.in
    include/Volcano/Scene/Registry.hpp
    src/Volcano/Scene/Common.cpp
    src/Volcano/Scene/Registry.cpp
)

configure_file(
    include/Volcano/Scene/Config.hpp.in
    include/Volcano/Scene/Config.hpp
)

target_include_directories(Scene PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(Scene)
set_target_properties(Scene PROPERTIES OUTPUT_NAME VolcanoScene)

target_link_libraries(Scene PUBLIC Base)

find_package(EnTT REQUIRED)
target_link_libraries(Scene PUBLIC EnTT::EnTT)
