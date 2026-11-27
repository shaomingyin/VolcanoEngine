
add_library(World STATIC
    include/Volcano/World/Config.h.in
    include/Volcano/World/Common.h
    include/Volcano/World/Config.h.in
    include/Volcano/World/Entity.h
    include/Volcano/World/Camera.h
    include/Volcano/World/BoxCollisionShape.h
    include/Volcano/World/CapsuleCollisionShape.h
    include/Volcano/World/CollisionShape.h
    include/Volcano/World/ColorEnvironment.h
    include/Volcano/World/DirectionalLight.h
    include/Volcano/World/Environment.h
    include/Volcano/World/Light.h
    include/Volcano/World/Material.h
    include/Volcano/World/Model.h
    include/Volcano/World/PointLight.h
    include/Volcano/World/RigidBody.h
    include/Volcano/World/Scene.h
    include/Volcano/World/SkyboxEnvironment.h
    include/Volcano/World/SpotLight.h
    include/Volcano/World/SphereCollisionShape.h
    include/Volcano/World/Transform.h
    include/Volcano/World/Transformable.h
    src/Volcano/World/Entity.cpp
    src/Volcano/World/Camera.cpp
    src/Volcano/World/BoxCollisionShape.cpp
    src/Volcano/World/CapsuleCollisionShape.cpp
    src/Volcano/World/CollisionShape.cpp
    src/Volcano/World/ColorEnvironment.cpp
    src/Volcano/World/DirectionalLight.cpp
    src/Volcano/World/Environment.cpp
    src/Volcano/World/Light.cpp
    src/Volcano/World/Material.cpp
    src/Volcano/World/Model.cpp
    src/Volcano/World/PointLight.cpp
    src/Volcano/World/RigidBody.cpp
    src/Volcano/World/Scene.cpp
    src/Volcano/World/SkyboxEnvironment.cpp
    src/Volcano/World/SpotLight.cpp
    src/Volcano/World/SphereCollisionShape.cpp
    src/Volcano/World/Transform.cpp
    src/Volcano/World/Transformable.cpp
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

volcano_link_qt(World Gui)
volcano_link_qt(World Qml)
volcano_setup_target(World)
