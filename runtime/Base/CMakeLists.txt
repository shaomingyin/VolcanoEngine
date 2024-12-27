
volcano_add_static_library(Base
    OUTPUT_NAME
        VolcanoBase
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/buddy2.h
        src/buddy2/buddy2.c

        include/Volcano/Config.h.in
        include/Volcano/Common.h
        include/Volcano/Config.h.in
        src/Volcano/Common.cpp
    PUBLIC_DEFINITIONS
        QT_NO_CONTEXTLESS_CONNECT
    PUBLIC_LINK_PACKAGES
        Async++ Async++
        microprofile microprofile::microprofile
        Eigen3 Eigen3::Eigen
    PUBLIC_QT_MODULES
        Concurrent
    PUBLIC_HEADERS
        include/buddy_alloc.h
        include/Volcano/Common.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Config.h
        DESTINATION include/Volcano
)
