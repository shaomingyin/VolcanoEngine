
add_library(Game STATIC
    include/Volcano/Game/AmbientLight.h
    include/Volcano/Game/BoxRigidBody.h
    include/Volcano/Game/Camera.h
    include/Volcano/Game/CapsuleRigidBody.h
    include/Volcano/Game/Config.h.in
    include/Volcano/Game/Common.h
    include/Volcano/Game/Component.h
    include/Volcano/Game/ConeRigidBody.h
    include/Volcano/Game/Config.h.in
    include/Volcano/Game/Context.h
    include/Volcano/Game/CylinderRigidBody.h
    include/Volcano/Game/DirectionalLight.h
    include/Volcano/Game/Dynamic.h
    include/Volcano/Game/Entity.h
    include/Volcano/Game/Glass.h
    include/Volcano/Game/Light.h
    include/Volcano/Game/Loadable.h
    include/Volcano/Game/Material.h
    include/Volcano/Game/Mirror.h
    include/Volcano/Game/Model.h
    include/Volcano/Game/Object.h
    include/Volcano/Game/Particle.h
    include/Volcano/Game/PointLight.h
    include/Volcano/Game/RigidBody.h
    include/Volcano/Game/Screen.h
    include/Volcano/Game/SphereRigidBody.h
    include/Volcano/Game/SpotLight.h
    include/Volcano/Game/StaticPlaneRigidBody.h
    include/Volcano/Game/Transform.h
    include/Volcano/Game/Transformable.h
    include/Volcano/Game/TriangleMeshRigidBody.h
    include/Volcano/Game/World.h
    src/Volcano/Game/AmbientLight.cpp
    src/Volcano/Game/BoxRigidBody.cpp
    src/Volcano/Game/Camera.cpp
    src/Volcano/Game/CapsuleRigidBody.cpp
    src/Volcano/Game/Component.cpp
    src/Volcano/Game/ConeRigidBody.cpp
    src/Volcano/Game/Context.cpp
    src/Volcano/Game/CylinderRigidBody.cpp
    src/Volcano/Game/DirectionalLight.cpp
    src/Volcano/Game/Dynamic.cpp
    src/Volcano/Game/Entity.cpp
    src/Volcano/Game/Glass.cpp
    src/Volcano/Game/Light.cpp
    src/Volcano/Game/Loadable.cpp
    src/Volcano/Game/Material.cpp
    src/Volcano/Game/Mirror.cpp
    src/Volcano/Game/Model.cpp
    src/Volcano/Game/Object.cpp
    src/Volcano/Game/Particle.cpp
    src/Volcano/Game/PointLight.cpp
    src/Volcano/Game/RigidBody.cpp
    src/Volcano/Game/Screen.cpp
    src/Volcano/Game/SphereRigidBody.cpp
    src/Volcano/Game/SpotLight.cpp
    src/Volcano/Game/StaticPlaneRigidBody.cpp
    src/Volcano/Game/Transform.cpp
    src/Volcano/Game/Transformable.cpp
    src/Volcano/Game/TriangleMeshRigidBody.cpp
    src/Volcano/Game/World.cpp
)

configure_file(
    include/Volcano/Game/Config.h.in
    include/Volcano/Game/Config.h
)

set_target_properties(Game PROPERTIES OUTPUT_NAME VolcanoGame)

target_include_directories(Game PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

target_link_libraries(Game PUBLIC Base)

volcano_link_qt(Game Quick)
volcano_link_package(Game Bullet Bullet::Bullet)
volcano_setup_target(Game)
