
volcano_add_auto_executable(Demo
    OUTPUT_NAME
        VolcanoDemo
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Demo/Common.h
        src/Volcano/Demo/Main.cpp
    PUBLIC_LINK_LIBRARIES
        System
)
