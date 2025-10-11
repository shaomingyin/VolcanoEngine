
add_library(World STATIC
    include/Volcano/World/Common.h
    include/Volcano/World/Config.h.in
    include/Volcano/World/Scene.h
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

target_link_libraries(World Base)

find_package(EnTT REQUIRED)
target_link_libraries(World EnTT::EnTT)

find_package(Bullet)
target_link_libraries(World Bullet::Bullet)
