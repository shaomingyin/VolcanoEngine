
add_library(Physics STATIC
    include/Volcano/Physics/Common.h
    include/Volcano/Physics/Config.h.in
    include/Volcano/Physics/System.h
    src/Volcano/Physics/System.cpp
)

configure_file(
    include/Volcano/Physics/Config.h.in
    include/Volcano/Physics/Config.h
)

set_target_properties(Physics PROPERTIES OUTPUT_NAME VolcanoPhysics)

target_include_directories(Physics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

target_link_libraries(Physics PUBLIC Base)

volcano_link_package(Physics Jolt Jolt::Jolt)
volcano_setup_target(Physics)
