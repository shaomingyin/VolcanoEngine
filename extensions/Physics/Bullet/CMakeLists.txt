
volcano_add_static_library(Bullet
    OUTPUT_NAME
        VolcanoPhysicsBullet
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Physics/Bullet/Config.h.in
        include/Volcano/Physics/Bullet/Common.h
        include/Volcano/Physics/Bullet/World.h
        src/Volcano/Physics/Bullet/World.cpp
    PUBLIC_LINK_LIBRARIES
        Physics
    PUBLIC_LINK_PACKAGES
        Bullet Bullet::Bullet
    PUBLIC_HEADERS
        include/Volcano/Physics/Bullet/Common.h
        include/Volcano/Physics/Bullet/World.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Physics/Bullet/Config.h
        DESTINATION include/Volcano/Physics/Bullet
)
