
add_library(Physics STATIC
    include/Volcano/Physics/Common.h
    include/Volcano/Physics/Config.h.in
    include/Volcano/Physics/System.h
    src/Volcano/Physics/Common.cpp
    src/Volcano/Physics/System.cpp
)

configure_file(
    include/Volcano/Physics/Config.h.in
    include/Volcano/Physics/Config.h
)

target_include_directories(Physics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(Physics)
set_target_properties(Physics PROPERTIES OUTPUT_NAME VolcanoPhysics)

target_link_libraries(Physics PUBLIC World)

find_package(Jolt)
target_link_libraries(Physics PUBLIC Jolt::Jolt)
