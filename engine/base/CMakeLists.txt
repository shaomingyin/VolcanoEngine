
add_volcano_shared_library(base
    OUTPUT_NAME
        Base
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PUBLIC_SOURCES
        include/Volcano/Common.hpp
    PRIVATE_SOURCES
        src/Volcano/Common.cpp
    CONFIGURE_FILES
        include/Volcano/Config.hpp.in
    )
