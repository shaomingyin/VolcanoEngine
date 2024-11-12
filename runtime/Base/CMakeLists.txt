
volcano_add_static_library(Base
    OUTPUT_NAME
        VolcanoBase
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
		include/buddy_alloc.h
        src/buddy_alloc/buddy_alloc.c

        include/Volcano/Cache.h
        include/Volcano/Color.h
        include/Volcano/Config.h.in
        include/Volcano/Common.h
        include/Volcano/Error.h
        include/Volcano/ScopeGuard.h
        include/Volcano/VersionNumber.h
        src/Volcano/Common.cpp
        src/Volcano/Error.cpp
        src/Volcano/VersionNumber.cpp
    PUBLIC_DEFINITIONS
        NOMINMAX
        VFSPP_ENABLE_MULTITHREADING
    PUBLIC_LINK_PACKAGES
        Eigen3 Eigen3::Eigen
        spdlog spdlog::spdlog
        microprofile microprofile::microprofile
        Taskflow Taskflow::Taskflow
        PhysFS physfs-static
    PUBLIC_HEADERS
        include/Volcano/Common.h
        include/Volcano/ScopeGuard.h
        include/Volcano/Resources.h
        include/Volcano/VersionNumber.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Config.h
        DESTINATION include/Volcano
)
