
add_library(Jolt STATIC
    include/Volcano/Jolt/Common.h
    include/Volcano/Jolt/Config.h.in
    include/Volcano/Jolt/System.h
    src/Volcano/Jolt/System.cpp
)

configure_file(
    include/Volcano/Jolt/Config.h.in
    include/Volcano/Jolt/Config.h
)

target_include_directories(Jolt PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(Jolt)
set_target_properties(Jolt PROPERTIES OUTPUT_NAME VolcanoJolt)

target_link_libraries(Jolt PUBLIC Physics)

find_package(Jolt)
target_link_libraries(Jolt PUBLIC Jolt::Jolt)
