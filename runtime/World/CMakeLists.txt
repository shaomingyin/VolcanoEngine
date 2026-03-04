
add_library(World STATIC
    include/Volcano/World/AmbientLight.h
    include/Volcano/World/BoxRigidBody.h
    include/Volcano/World/Camera.h
    include/Volcano/World/CapsuleRigidBody.h
    include/Volcano/World/Config.h.in
    include/Volcano/World/Common.h
    include/Volcano/World/Component.h
    include/Volcano/World/ConeRigidBody.h
    include/Volcano/World/Config.h.in
    include/Volcano/World/Context.h
    include/Volcano/World/CylinderRigidBody.h
    include/Volcano/World/DirectionalLight.h
    include/Volcano/World/Dynamic.h
    include/Volcano/World/Entity.h
    include/Volcano/World/Glass.h
    include/Volcano/World/Light.h
    include/Volcano/World/Loadable.h
    include/Volcano/World/Material.h
    include/Volcano/World/Mirror.h
    include/Volcano/World/Model.h
    include/Volcano/World/Object.h
    include/Volcano/World/Particle.h
    include/Volcano/World/PointLight.h
    include/Volcano/World/RigidBody.h
    include/Volcano/World/Scene.h
    include/Volcano/World/Screen.h
    include/Volcano/World/SphereRigidBody.h
    include/Volcano/World/SpotLight.h
    include/Volcano/World/StaticPlaneRigidBody.h
    include/Volcano/World/Transformable.h
    include/Volcano/World/TriangleMeshRigidBody.h
    src/Volcano/World/AmbientLight.cpp
    src/Volcano/World/BoxRigidBody.cpp
    src/Volcano/World/Camera.cpp
    src/Volcano/World/CapsuleRigidBody.cpp
    src/Volcano/World/Component.cpp
    src/Volcano/World/ConeRigidBody.cpp
    src/Volcano/World/Context.cpp
    src/Volcano/World/CylinderRigidBody.cpp
    src/Volcano/World/DirectionalLight.cpp
    src/Volcano/World/Dynamic.cpp
    src/Volcano/World/Entity.cpp
    src/Volcano/World/Glass.cpp
    src/Volcano/World/Light.cpp
    src/Volcano/World/Loadable.cpp
    src/Volcano/World/Material.cpp
    src/Volcano/World/Mirror.cpp
    src/Volcano/World/Model.cpp
    src/Volcano/World/Object.cpp
    src/Volcano/World/Particle.cpp
    src/Volcano/World/PointLight.cpp
    src/Volcano/World/RigidBody.cpp
    src/Volcano/World/Scene.cpp
    src/Volcano/World/Screen.cpp
    src/Volcano/World/SphereRigidBody.cpp
    src/Volcano/World/SpotLight.cpp
    src/Volcano/World/StaticPlaneRigidBody.cpp
    src/Volcano/World/Transformable.cpp
    src/Volcano/World/TriangleMeshRigidBody.cpp
)

configure_file(
    include/Volcano/World/Config.h.in
    include/Volcano/World/Config.h
)

set_target_properties(World PROPERTIES OUTPUT_NAME VolcanoWorld)

target_include_directories(World PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

target_link_libraries(World PUBLIC Base)

volcano_link_qt(World Quick)
volcano_link_package(World Jolt Jolt::Jolt)
volcano_setup_target(World)
