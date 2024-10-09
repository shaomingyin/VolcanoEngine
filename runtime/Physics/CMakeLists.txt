
volcano_add_shared_library(Physics
    OUTPUT_NAME
        VolcanoPhysics
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Physics/Config.h.in
        include/Volcano/Physics/Common.h
        include/Volcano/Physics/World.h
        src/Volcano/Physics/World.cpp
    PUBLIC_LINK_LIBRARIES
        Base
    PUBLIC_HEADERS
        include/Volcano/Physics/Common.h
        include/Volcano/Physics/World.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Physics/Config.h
        DESTINATION include/Volcano/Physics
)
