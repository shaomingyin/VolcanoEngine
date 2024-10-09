
volcano_add_static_library(Base
    OUTPUT_NAME
        VolcanoBase
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Config.h.in
        include/Volcano/Common.h
        include/Volcano/Plugin.h
        include/Volcano/PluginHolder.h
        include/Volcano/PluginManager.h
        include/Volcano/ScopeGuard.h
        include/Volcano/VersionNumber.h
        src/Volcano/PluginHolder.cpp
        src/Volcano/PluginManager.cpp
        src/Volcano/VersionNumber.cpp
    PUBLIC_LINK_PACKAGES
        Eigen3 Eigen3::Eigen
        spdlog spdlog::spdlog
        libuv uv
    PUBLIC_HEADERS
        include/Volcano/Common.h
        include/Volcano/ScopeGuard.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Config.h
        DESTINATION include/Volcano
)
