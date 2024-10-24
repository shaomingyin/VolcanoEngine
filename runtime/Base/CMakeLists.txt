
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
        include/Volcano/Eigen.h
        include/Volcano/Error.h
		include/Volcano/ResourceManager.h
        include/Volcano/ScopeGuard.h
        include/Volcano/SDL_Initializer.h
        include/Volcano/VersionNumber.h
        src/Volcano/Error.cpp
        src/Volcano/ResourceManager.cpp
        src/Volcano/VersionNumber.cpp
    PUBLIC_DEFINITIONS
        VFSPP_ENABLE_MULTITHREADING
    PUBLIC_LINK_PACKAGES
        Eigen3 Eigen3::Eigen
        fmt fmt::fmt
        PalSigslot Pal::Sigslot
        SDL2 SDL2::SDL2
        microprofile microprofile::microprofile
        miniz miniz::miniz
    PUBLIC_HEADERS
        include/Volcano/Common.h
        include/Volcano/ScopeGuard.h
        include/Volcano/VersionNumber.h
        include/Volcano/ResourceManager.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Config.h
        DESTINATION include/Volcano
)
