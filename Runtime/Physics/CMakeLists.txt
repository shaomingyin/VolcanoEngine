
volcano_glob_sources(PHYSICS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR})
volcano_define_shared_library(Physics
    OUTPUT_NAME
        VolcanoPhysics
    PRIVATE_SOURCES
        ${PHYSICS_SOURCES}
    PRIVATE_DEFINITIONS
        VOLCANO_PHYSICS
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PUBLIC_LINK_LIBRARIES
        Base
    )
