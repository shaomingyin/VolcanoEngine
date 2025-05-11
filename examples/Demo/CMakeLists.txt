
volcano_add_static_library(Demo
    OUTPUT_NAME
        VolcanoDemo
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Demo/Common.h
        include/Volcano/Demo/Game.h
        src/Volcano/Demo/Game.cpp
    PUBLIC_LINK_LIBRARIES
        Framework
)
