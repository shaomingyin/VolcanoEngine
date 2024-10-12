
volcano_add_static_library(Game
    OUTPUT_NAME
        VolcanoGame
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Game/Config.h.in
        include/Volcano/Game/Common.h
        include/Volcano/Game/Context.h
        include/Volcano/Game/Object.h
        include/Volcano/Game/World.h
        include/Volcano/Game/Scene.h
        include/Volcano/Game/Actor.h
        include/Volcano/Game/Entity.h
        include/Volcano/Game/Component.h
        include/Volcano/Game/Light.h
        include/Volcano/Game/DirectionalLight.h
        include/Volcano/Game/PointLight.h
        include/Volcano/Game/SpotLight.h
        src/Volcano/Game/Context.cpp
        src/Volcano/Game/Object.cpp
        src/Volcano/Game/World.cpp
        src/Volcano/Game/Scene.cpp
        src/Volcano/Game/Actor.cpp
        src/Volcano/Game/Entity.cpp
        src/Volcano/Game/Component.cpp
        src/Volcano/Game/Light.cpp
        src/Volcano/Game/DirectionalLight.cpp
        src/Volcano/Game/PointLight.cpp
        src/Volcano/Game/SpotLight.cpp
    PUBLIC_LINK_LIBRARIES
        Base
    PUBLIC_LINK_PACKAGES
        EnTT EnTT::EnTT
        Bullet Bullet::Bullet
    PUBLIC_HEADERS
        include/Volcano/Game/Common.h
        include/Volcano/Game/Object.h
        include/Volcano/Game/World.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Game/Config.h
        DESTINATION include/Volcano/Game
)
