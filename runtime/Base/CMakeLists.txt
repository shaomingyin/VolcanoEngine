
volcano_add_static_library(Base
    OUTPUT_NAME
        VolcanoBase
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Eigen.h

		include/buddy_alloc.h
        src/buddy_alloc/buddy_alloc.c

        include/Volcano/Cache.h
        include/Volcano/Color.h
        include/Volcano/Config.h.in
        include/Volcano/Common.h
        include/Volcano/ScopeGuard.h
        include/Volcano/VersionNumber.h
        src/Volcano/Common.cpp
        src/Volcano/VersionNumber.cpp
    PUBLIC_DEFINITIONS
        NOMINMAX
        VFSPP_ENABLE_MULTITHREADING
    PUBLIC_LINK_PACKAGES
        PalSigslot Pal::Sigslot
        Async++ Async++
        Eigen3 Eigen3::Eigen
        spdlog spdlog::spdlog
        microprofile microprofile::microprofile
    PUBLIC_HEADERS
        include/buddy_alloc.h
        include/Eigen.h
        include/Volcano/Common.h
        include/Volcano/ScopeGuard.h
        include/Volcano/VersionNumber.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Config.h
        DESTINATION include/Volcano
)
