
add_library(Scene STATIC
    include/Volcano/Scene/Common.h
    include/Volcano/Scene/Config.h.in
    include/Volcano/Scene/Registry.h
    src/Volcano/Scene/Common.cpp
    src/Volcano/Scene/Registry.cpp
)

configure_file(
    include/Volcano/Scene/Config.h.in
    include/Volcano/Scene/Config.h
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
