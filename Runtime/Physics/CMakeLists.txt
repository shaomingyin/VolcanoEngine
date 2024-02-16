
find_package(Bullet CONFIG REQUIRED)

volcano_glob_sources(PHYSICS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR})
volcano_define_shared_library(Physics
    OUTPUT_NAME
        VolcanoPhysics
    PRIVATE_SOURCES
        ${PHYSICS_SOURCES}
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
        ${BULLET_INCLUDE_DIR}
    PUBLIC_LINK_LIBRARIES
		Base
        ${BULLET_LIBRARIES}
    )
