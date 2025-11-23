
add_library(Game STATIC
    include/Volcano/Game/Common.hpp
    include/Volcano/Game/Component.hpp
    include/Volcano/Game/Config.hpp.in
    include/Volcano/Game/Context.hpp
    include/Volcano/Game/Entity.hpp
    include/Volcano/Game/World.hpp
    include/Volcano/Game/Transform.hpp
    src/Volcano/Game/Common.cpp
    src/Volcano/Game/Component.cpp
    src/Volcano/Game/Context.cpp
    src/Volcano/Game/Entity.cpp
    src/Volcano/Game/World.cpp
)

configure_file(
    include/Volcano/Game/Config.hpp.in
    include/Volcano/Game/Config.hpp
)

target_include_directories(Game PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(Game)
set_target_properties(Game PROPERTIES OUTPUT_NAME VolcanoGame)

target_link_libraries(Game PUBLIC Base)

find_package(Async++ REQUIRED)
target_link_libraries(Game PUBLIC Async++)

find_package(PhysFS REQUIRED)
target_link_libraries(Game PUBLIC physfs-static)

find_package(Bullet REQUIRED)
target_link_libraries(Game PUBLIC Bullet::Bullet)
