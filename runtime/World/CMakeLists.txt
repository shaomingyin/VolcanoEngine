
volcano_add_static_library(World
    OUTPUT_NAME
        VolcanoWorld
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
		include/Volcano/World/BoxRigidBody.h
		include/Volcano/World/CapsuleRigidBody.h
        include/Volcano/World/Common.h
		include/Volcano/World/ConeRigidBody.h
        include/Volcano/World/Config.h.in
		include/Volcano/World/CylinderRigidBody.h
        include/Volcano/World/Inherency.h
		include/Volcano/World/RigidBody.h
		include/Volcano/World/Scene.h
		include/Volcano/World/SphereRigidBody.h
        include/Volcano/World/StaticPlaneRigidBody.h
        include/Volcano/World/Transformable.h
		include/Volcano/World/TriangleMeshRigidBody.h
		src/Volcano/World/BoxRigidBody.cpp
		src/Volcano/World/CapsuleRigidBody.cpp
		src/Volcano/World/ConeRigidBody.cpp
		src/Volcano/World/CylinderRigidBody.cpp
        src/Volcano/World/Inherency.cpp
		src/Volcano/World/RigidBody.cpp
        src/Volcano/World/Scene.cpp
		src/Volcano/World/SphereRigidBody.cpp
		src/Volcano/World/StaticPlaneRigidBody.cpp
        src/Volcano/World/Transformable.cpp
		src/Volcano/World/TriangleMeshRigidBody.cpp
    PUBLIC_LINK_LIBRARIES
        Base
    PUBLIC_LINK_PACKAGES
        EnTT EnTT::EnTT
		Bullet Bullet::Bullet
    PUBLIC_HEADERS
        include/Volcano/World/Common.h
        include/Volcano/World/Scene.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/World/Config.h
        DESTINATION include/Volcano/World
)
