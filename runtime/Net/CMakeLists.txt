
volcano_add_static_library(Net
    OUTPUT_NAME
        VolcanoNet
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Net/Base.h
        include/Volcano/Net/Client.h
        include/Volcano/Net/Common.h
        include/Volcano/Net/Config.h.in
        include/Volcano/Net/Server.h
        src/Volcano/Net/Base.cpp
        src/Volcano/Net/Client.cpp
        src/Volcano/Net/Server.cpp
    PUBLIC_LINK_LIBRARIES
        Game
    PUBLIC_LINK_PACKAGES
        enet enet::enet
    PUBLIC_HEADERS
		include/Volcano/Net/Base.h
        include/Volcano/Net/Client.h
        include/Volcano/Net/Common.h
        include/Volcano/Net/Server.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Net/Config.h
        DESTINATION include/Volcano/Net
)
