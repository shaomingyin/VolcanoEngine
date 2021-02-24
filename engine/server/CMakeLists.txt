
add_volcano_executable(server
    OUTPUT_NAME
        Server
    DEPENDENCIES
        resources
        game
        base
    PUBLIC_LINK_LIBRARIES
        game
        base
    PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Server/Common.hpp
        src/Volcano/Server/Main.cpp
    )
