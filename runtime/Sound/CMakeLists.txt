
volcano_add_static_library(Sound
    OUTPUT_NAME
        VolcanoSound
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Sound/Config.h.in
        include/Volcano/Sound/Common.h
        include/Volcano/Sound/Space.h
        src/Volcano/Sound/Space.cpp
    PUBLIC_LINK_LIBRARIES
        Base
    PUBLIC_LINK_PACKAGES
        OpenAL OpenAL::OpenAL
    PUBLIC_HEADERS
        include/Volcano/Sound/Common.h
        include/Volcano/Sound/Space.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Sound/Config.h
        DESTINATION include/Volcano/Sound
)
