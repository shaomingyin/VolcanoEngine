
add_library(World STATIC
    include/Volcano/World/Audio.h
    include/Volcano/World/Camera.h
    include/Volcano/World/Common.h
    include/Volcano/World/Config.h.in
    include/Volcano/World/Kinetics.h
    include/Volcano/World/Light.h
    include/Volcano/World/Scene.h
    include/Volcano/World/Screen.h
    include/Volcano/World/Transform.h
    include/Volcano/World/Visual.h
    src/Volcano/World/Camera.cpp
    src/Volcano/World/Common.cpp
    src/Volcano/World/Scene.cpp
)

configure_file(
    include/Volcano/World/Config.h.in
    include/Volcano/World/Config.h
)

target_include_directories(World PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(World)
set_target_properties(World PROPERTIES OUTPUT_NAME VolcanoWorld)

target_link_libraries(World PUBLIC Base)

find_package(Async++ REQUIRED)
target_link_libraries(World PUBLIC Async++)

find_package(EnTT REQUIRED)
target_link_libraries(World PUBLIC EnTT::EnTT)

find_package(Jolt REQUIRED)
target_link_libraries(World PUBLIC Jolt::Jolt)
