
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
        include/Volcano/System/ServerBase.h
        include/Volcano/System/ServerProxy.h
        src/Volcano/System/Client.cpp
        src/Volcano/System/Server.cpp
        src/Volcano/System/ServerBase.cpp
        src/Volcano/System/ServerProxy.cpp
    PUBLIC_LINK_LIBRARIES
        Game
        Graphics
    PUBLIC_LINK_PACKAGES
        enet enet::enet
    PUBLIC_HEADERS
        include/Volcano/System/Common.h
        include/Volcano/System/Client.h
        include/Volcano/System/Server.h
        include/Volcano/System/ServerBase.h
        include/Volcano/System/ServerProxy.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/System/Config.h
        DESTINATION include/Volcano/System
)
