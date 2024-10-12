
volcano_add_static_library(System
    OUTPUT_NAME
        VolcanoSystem
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/System/Config.h.in
        include/Volcano/System/Common.h
        include/Volcano/System/Client.h
        include/Volcano/System/Server.h
        include/Volcano/System/MultiPlayer.h
        include/Volcano/System/SinglePlayer.h
        src/Volcano/System/Client.cpp
        src/Volcano/System/Server.cpp
        src/Volcano/System/MultiPlayer.cpp
        src/Volcano/System/SinglePlayer.cpp
    PUBLIC_LINK_LIBRARIES
        Game
        Graphics
        Sound
        Base
    PUBLIC_LINK_PACKAGES
        enet enet::enet
        glfw3 glfw
    PUBLIC_HEADERS
        include/Volcano/System/Common.h
        include/Volcano/System/Client.h
        include/Volcano/System/Server.h
        include/Volcano/System/MultiPlayer.h
        include/Volcano/System/SinglePlayer.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/System/Config.h
        DESTINATION include/Volcano/System
)
