
volcano_add_executable(Server
    OUTPUT_NAME
        VolcanoServer
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        Main.cpp
    PUBLIC_LINK_LIBRARIES
		Framework
)
