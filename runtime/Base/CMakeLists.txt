
volcano_add_static_library(Base
    OUTPUT_NAME
        VolcanoBase
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/nlohmann_json.h
        src/nlohmann_json.cpp
        include/Volcano/Color.h
        include/Volcano/Config.h.in
        include/Volcano/Common.h
        include/Volcano/VersionNumber.h
        src/Volcano/VersionNumber.cpp
    PUBLIC_LINK_PACKAGES
        rttr RTTR::Core
        Async++ Async++
        Eigen3 Eigen3::Eigen
        PalSigslot Pal::Sigslot
        nlohmann_json nlohmann_json::nlohmann_json
        SDL3 SDL3::SDL3
        microprofile microprofile::microprofile
    PUBLIC_HEADERS
        include/Volcano/Common.h
        include/Volcano/ScopeGuard.h
        include/Volcano/VersionNumber.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Config.h
        DESTINATION include/Volcano
    PUBLIC_DEFINITIONS
        NOMINMAX
)
