
volcano_add_static_library(Bullet
    OUTPUT_NAME
        VolcanoBullet
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Bullet/Config.h.in
        include/Volcano/Bullet/Common.h
        include/Volcano/Bullet/World.h
        src/Volcano/Bullet/World.cpp
    PUBLIC_LINK_LIBRARIES
        Physics
    PUBLIC_LINK_PACKAGES
        Bullet Bullet::Bullet
    PUBLIC_HEADERS
        include/Volcano/Bullet/Common.h
        include/Volcano/Bullet/World.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Bullet/Config.h
        DESTINATION include/Volcano/Bullet
)
