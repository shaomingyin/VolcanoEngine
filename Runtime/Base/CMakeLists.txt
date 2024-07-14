
find_package(Eigen3 REQUIRED)

volcano_glob_sources(BASE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR})
volcano_define_shared_library(Base
    OUTPUT_NAME
        VolcanoBase
    PRIVATE_SOURCES
        ${BASE_SOURCES}
    PRIVATE_DEFINITIONS
        VOLCANO_BASE
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PUBLIC_QT_MODULES
        Concurrent
        Gui
    PUBLIC_LINK_LIBRARIES
        Eigen3::Eigen
    )
