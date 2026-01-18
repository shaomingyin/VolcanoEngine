
add_library(Game STATIC
    include/Volcano/Game/Camera.hpp
    include/Volcano/Game/Common.hpp
    include/Volcano/Game/Component.hpp
    include/Volcano/Game/Config.hpp.in
    include/Volcano/Game/DirectionalLight.hpp
    include/Volcano/Game/Dynamic.hpp
    include/Volcano/Game/Entity.hpp
    include/Volcano/Game/Light.hpp
    include/Volcano/Game/Object.hpp
    include/Volcano/Game/PointLight.hpp
    include/Volcano/Game/Scene.hpp
    include/Volcano/Game/Screen.hpp
    include/Volcano/Game/SpotLight.hpp
    include/Volcano/Game/Transform.hpp
    include/Volcano/Game/ViewBuilder.hpp
    include/Volcano/Game/World.hpp
    src/Volcano/Game/Camera.cpp
    src/Volcano/Game/Common.cpp
    src/Volcano/Game/Component.cpp
    src/Volcano/Game/DirectionalLight.cpp
    src/Volcano/Game/Dynamic.cpp
    src/Volcano/Game/Entity.cpp
    src/Volcano/Game/Light.cpp
    src/Volcano/Game/PointLight.cpp
    src/Volcano/Game/Scene.cpp
    src/Volcano/Game/Screen.cpp
    src/Volcano/Game/SpotLight.cpp
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

target_link_libraries(Game PUBLIC Graphics Gui)

find_package(Async++ REQUIRED)
target_link_libraries(Game PUBLIC Async++)

find_package(PhysFS REQUIRED)
target_link_libraries(Game PUBLIC physfs-static)

find_package(Bullet REQUIRED)
target_link_libraries(Game PUBLIC Bullet::Bullet)
