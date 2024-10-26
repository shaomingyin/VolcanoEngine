
volcano_add_static_library(System
    OUTPUT_NAME
        VolcanoSystem
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/System/Base.h
        include/Volcano/System/Config.h.in
        include/Volcano/System/Common.h
        include/Volcano/System/Frontend.h
        include/Volcano/System/JsonUtils.h
        include/Volcano/System/LocalPlayer.h
        include/Volcano/System/MultiPlayerClient.h
        include/Volcano/System/MultiPlayerServer.h
        include/Volcano/System/Persistence.h
        include/Volcano/System/SinglePlayer.h
        src/Volcano/System/Base.cpp
        src/Volcano/System/Frontend.cpp
        src/Volcano/System/JsonUtils.cpp
        src/Volcano/System/LocalPlayer.cpp
        src/Volcano/System/MultiPlayerClient.cpp
        src/Volcano/System/MultiPlayerServer.cpp
        src/Volcano/System/Persistence.cpp
        src/Volcano/System/SinglePlayer.cpp
    PUBLIC_LINK_LIBRARIES
        Net
        Graphics
        Sound
    PUBLIC_LINK_PACKAGES
        nlohmann_json nlohmann_json::nlohmann_json
    PUBLIC_HEADERS
        include/Volcano/System/Common.h
        include/Volcano/System/Frontend.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/System/Config.h
        DESTINATION include/Volcano/System
)
