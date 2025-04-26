
volcano_add_executable(Server
    OUTPUT_NAME
        VolcanoServer
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Server/Config.h.in
        include/Volcano/Server/Common.h
        src/Volcano/Server/Main.cpp
    PUBLIC_LINK_LIBRARIES
		Physics
        World
		Schema
    PUBLIC_HEADERS
        include/Volcano/Server/Common.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Server/Config.h
        DESTINATION include/Volcano/Server
)
