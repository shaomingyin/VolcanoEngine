
add_library(game STATIC)

add_dependencies(game
    base
    )

target_link_libraries(game
    PUBLIC
        base
    )

target_include_directories(game
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

target_sources(game
    PRIVATE
        include/Volcano/Game/Camera.hpp
        include/Volcano/Game/Common.hpp
        include/Volcano/Game/Component.hpp
        include/Volcano/Game/Context.hpp
        include/Volcano/Game/DirectionalLight.hpp
        include/Volcano/Game/DynamicWorld.hpp
        include/Volcano/Game/Entity.hpp
        include/Volcano/Game/Light.hpp
        include/Volcano/Game/Model.hpp
        include/Volcano/Game/PerpectiveCamera.hpp
        include/Volcano/Game/PointLight.hpp
        include/Volcano/Game/Scene.hpp
        include/Volcano/Game/SpotLight.hpp
        include/Volcano/Game/World.hpp
        src/Volcano/Game/Camera.cpp
        src/Volcano/Game/Common.cpp
        src/Volcano/Game/Component.cpp
        src/Volcano/Game/Context.cpp
        src/Volcano/Game/DirectionalLight.cpp
        src/Volcano/Game/DynamicWorld.cpp
        src/Volcano/Game/Entity.cpp
        src/Volcano/Game/Light.cpp
        src/Volcano/Game/Model.cpp
        src/Volcano/Game/PointLight.cpp
        src/Volcano/Game/Scene.cpp
        src/Volcano/Game/SpotLight.cpp
        src/Volcano/Game/World.cpp
    )

# BulletPhysics
include(FindBullet)
if(BULLET_FOUND)
    target_include_directories(game PUBLIC ${BULLET_INCLUDE_DIRS})
    target_link_libraries(game PUBLIC ${BULLET_LIBRARIES})
else()
    message(FATAL_ERROR "Bullet physics not found.")
endif()

volcano_target_post_setup(game)
