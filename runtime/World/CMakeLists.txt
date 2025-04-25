
volcano_add_static_library(World
    OUTPUT_NAME
        VolcanoWorld
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/World/Basic.h
        include/Volcano/World/Config.h.in
        include/Volcano/World/Common.h
        include/Volcano/World/Component.h
        include/Volcano/World/Entity.h
        include/Volcano/World/Scene.h
        src/Volcano/World/Basic.cpp
        src/Volcano/World/Component.cpp
        src/Volcano/World/Scene.cpp
        src/Volcano/World/Entity.cpp
    PUBLIC_LINK_LIBRARIES
        Base
    PUBLIC_LINK_PACKAGES
        EnTT EnTT::EnTT
    PUBLIC_HEADERS
        include/Volcano/World/Common.h
        include/Volcano/World/Object.h
        include/Volcano/World/World.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/World/Config.h
        DESTINATION include/Volcano/World
)
