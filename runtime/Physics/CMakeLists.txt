
volcano_add_static_library(Physics
    OUTPUT_NAME
        VolcanoPhysics
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Physics/Config.h.in
        include/Volcano/Physics/Common.h
        include/Volcano/Physics/World.h
        src/Volcano/Physics/World.cpp
    PUBLIC_LINK_LIBRARIES
        Base
    PUBLIC_LINK_PACKAGES
        Bullet Bullet::Bullet
)
