
volcano_add_static_library(Base
    OUTPUT_NAME
        VolcanoBase
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Color.h
        include/Volcano/Config.h.in
        include/Volcano/Common.h
        include/Volcano/ScopeGuard.h
        include/Volcano/SharedLibrary.h
        include/Volcano/VersionNumber.h
        src/Volcano/SharedLibrary.cpp
        src/Volcano/VersionNumber.cpp
    PUBLIC_LINK_PACKAGES
        Eigen3 Eigen3::Eigen
        fmt fmt::fmt
        PalSigslot Pal::Sigslot
        SDL3 SDL3::SDL3
        microprofile microprofile::microprofile
    PUBLIC_HEADERS
        include/Volcano/Common.h
        include/Volcano/ScopeGuard.h
        include/Volcano/VersionNumber.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Config.h
        DESTINATION include/Volcano
)
