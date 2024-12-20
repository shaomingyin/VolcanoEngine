
volcano_add_static_library(Physics
    OUTPUT_NAME
        VolcanoPhysics
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Physics/BoxRigidBody.h
        include/Volcano/Physics/CapsuleRigidBody.h
        include/Volcano/Physics/Common.h
        include/Volcano/Physics/ConeRigidBody.h
        include/Volcano/Physics/Config.h.in
        include/Volcano/Physics/CylinderRigidBody.h
        include/Volcano/Physics/RigidBody.h
        include/Volcano/Physics/Simulator.h
        include/Volcano/Physics/SphereRigidBody.h
        include/Volcano/Physics/StaticPlaneRigidBody.h
        include/Volcano/Physics/TriangleMeshRigidBody.h
        src/Volcano/Physics/BoxRigidBody.cpp
        src/Volcano/Physics/CapsuleRigidBody.cpp
        src/Volcano/Physics/ConeRigidBody.cpp
        src/Volcano/Physics/CylinderRigidBody.cpp
        src/Volcano/Physics/RigidBody.cpp
        src/Volcano/Physics/Simulator.cpp
        src/Volcano/Physics/SphereRigidBody.cpp
        src/Volcano/Physics/StaticPlaneRigidBody.cpp
        src/Volcano/Physics/TriangleMeshRigidBody.cpp
    PUBLIC_LINK_LIBRARIES
        World
    PUBLIC_LINK_PACKAGES
        Bullet Bullet::Bullet
    PUBLIC_HEADERS
        include/Volcano/Physics/Common.h
        include/Volcano/Physics/Simulator.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Physics/Config.h
        DESTINATION include/Volcano/Physics
)
