
volcano_add_static_library(Schema
    OUTPUT_NAME
        VolcanoSchema
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Schema/Config.h.in
        include/Volcano/Schema/Common.h
        src/Volcano/Schema/Common.cpp
    PUBLIC_LINK_LIBRARIES
        Base
    PUBLIC_HEADERS
        include/Volcano/Schema/Common.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Schema/Config.h
        DESTINATION include/Volcano/Schema
)
