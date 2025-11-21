
add_library(World STATIC
    include/Volcano/World/Common.hpp
    include/Volcano/World/Config.hpp.in
    include/Volcano/World/Context.hpp
    include/Volcano/World/Entity.hpp
    include/Volcano/World/Scene.hpp
    include/Volcano/World/Transform.hpp
    src/Volcano/World/Common.cpp
    src/Volcano/World/Context.cpp
    src/Volcano/World/Entity.cpp
    src/Volcano/World/Scene.cpp
)

configure_file(
    include/Volcano/World/Config.hpp.in
    include/Volcano/World/Config.hpp
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

find_package(PhysFS REQUIRED)
target_link_libraries(World PUBLIC physfs-static)

find_package(Bullet REQUIRED)
target_link_libraries(World PUBLIC Bullet::Bullet)
