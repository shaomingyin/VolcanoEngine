
volcano_static_library(framework-base
    DEPENDENCIES
        drivers
        game
    PUBLIC_LINK_LIBRARIES
        drivers
        game
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Framework/Common.hpp
        include/Volcano/Framework/Config.hpp.in
        include/Volcano/Framework/Server.hpp
        src/Volcano/Framework/Common.cpp
        src/Volcano/Framework/Server.cpp
    )

configure_file(
    include/Volcano/Framework/Config.hpp.in
    include/Volcano/Framework/Config.hpp
    )

volcano_static_library(framework-client
    DEPENDENCIES
        framework-base
    PUBLIC_LINK_LIBRARIES
        framework-base
    PRIVATE_SOURCES
        include/Volcano/Framework/Client.hpp
        include/Volcano/Framework/MultiPlayer.hpp
        include/Volcano/Framework/SinglePlayer.hpp
        src/Volcano/Framework/Client.cpp
        src/Volcano/Framework/MultiPlayer.cpp
        src/Volcano/Framework/SinglePlayer.cpp
    )
