
add_volcano_shared_library(input
    OUTPUT_NAME
        Input
    DEPENDENCIES
        base
    PUBLIC_LINK_LIBRARIES
        base
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Input/Common.hpp
        src/Volcano/Input/Common.cpp
    )
