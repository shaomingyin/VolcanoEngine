
# BulletPhysics
include(FindBullet)
if(NOT BULLET_FOUND)
    message(FATAL_ERROR "Bullet physics not found.")
endif()

volcano_static_library(game
    DEPENDENCIES
        base
    PUBLIC_LINK_LIBRARIES
        base
        ${BULLET_LIBRARIES}
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
        ${BULLET_INCLUDE_DIRS}
    PRIVATE_SOURCES
        include/Volcano/Game/Camera.hpp
        include/Volcano/Game/Common.hpp
        include/Volcano/Game/Component.hpp
        include/Volcano/Game/ContextBase.hpp
        include/Volcano/Game/DirectionalLight.hpp
        include/Volcano/Game/DynamicWorld.hpp
        include/Volcano/Game/Entity.hpp
        include/Volcano/Game/Light.hpp
        include/Volcano/Game/Model.hpp
        include/Volcano/Game/Object.hpp
        include/Volcano/Game/OrthoCamera.hpp
        include/Volcano/Game/PerpectiveCamera.hpp
        include/Volcano/Game/PointLight.hpp
        include/Volcano/Game/Scene.hpp
        include/Volcano/Game/Screen.hpp
        include/Volcano/Game/SpotLight.hpp
        include/Volcano/Game/World.hpp
        src/Volcano/Game/Camera.cpp
        src/Volcano/Game/Common.cpp
        src/Volcano/Game/Component.cpp
        src/Volcano/Game/ContextBase.cpp
        src/Volcano/Game/DirectionalLight.cpp
        src/Volcano/Game/DynamicWorld.cpp
        src/Volcano/Game/Entity.cpp
        src/Volcano/Game/Light.cpp
        src/Volcano/Game/Model.cpp
        src/Volcano/Game/Object.cpp
        src/Volcano/Game/OrthoCamera.cpp
        src/Volcano/Game/PerpectiveCamera.cpp
        src/Volcano/Game/PointLight.cpp
        src/Volcano/Game/Scene.cpp
        src/Volcano/Game/Screen.cpp
        src/Volcano/Game/SpotLight.cpp
        src/Volcano/Game/World.cpp

        include/Volcano/Game/Templates/Common.hpp
        src/Volcano/Game/Templates/Common.cpp

        include/Volcano/Game/Templates/FPS/Common.hpp
        include/Volcano/Game/Templates/FPS/Context.hpp
        src/Volcano/Game/Templates/FPS/Common.cpp
        src/Volcano/Game/Templates/FPS/Context.cpp
    )
