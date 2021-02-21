
add_volcano_shared_library(sound
    OUTPUT_NAME
        Sound
    DEPENDENCIES
        base
    PUBLIC_LINK_LIBRARIES
        base
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Sound/Common.hpp
        src/Volcano/Sound/Common.cpp
    )
