
volcano_add_static_library(Base
    OUTPUT_NAME
        VolcanoBase
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Cache.h
        include/Volcano/Color.h
        include/Volcano/Config.h.in
        include/Volcano/Common.h
        include/Volcano/Eigen.h
        include/Volcano/Error.h
        include/Volcano/Rect.h
		include/Volcano/Resources.h
        include/Volcano/ScopeGuard.h
        include/Volcano/Size.h
        include/Volcano/VersionNumber.h
        src/Volcano/Error.cpp
        src/Volcano/Resources.cpp
        src/Volcano/VersionNumber.cpp
    PUBLIC_DEFINITIONS
        NOMINMAX
        VFSPP_ENABLE_MULTITHREADING
    PUBLIC_LINK_PACKAGES
        Eigen3 Eigen3::Eigen
        spdlog spdlog::spdlog
        PalSigslot Pal::Sigslot
        microprofile microprofile::microprofile
        miniz miniz::miniz
        Taskflow Taskflow::Taskflow
    PUBLIC_HEADERS
        include/Volcano/Common.h
        include/Volcano/ScopeGuard.h
        include/Volcano/Resources.h
        include/Volcano/VersionNumber.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Config.h
        DESTINATION include/Volcano
)
