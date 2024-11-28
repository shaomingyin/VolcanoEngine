
volcano_add_static_library(Network
    OUTPUT_NAME
        VolcanoNetwork
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Network/Client.h
        include/Volcano/Network/Config.h.in
        include/Volcano/Network/Common.h
        include/Volcano/Network/Server.h
        src/Volcano/Network/Client.cpp
        src/Volcano/Network/Server.cpp
    PUBLIC_LINK_LIBRARIES
        World
    PUBLIC_LINK_PACKAGES
        enet enet::enet
    PUBLIC_HEADERS
        include/Volcano/Network/Common.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Network/Config.h
        DESTINATION include/Volcano/Network
)
