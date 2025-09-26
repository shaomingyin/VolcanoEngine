
volcano_add_shared_library(Acoustics
    OUTPUT_NAME
        VolcanoAcoustics
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Acoustics/Config.h.in
        include/Volcano/Acoustics/Common.h
        include/Volcano/Acoustics/Space.h
        src/Volcano/Acoustics/Space.cpp
    PUBLIC_LINK_LIBRARIES
        Base
    PUBLIC_LINK_PACKAGES
        OpenAL OpenAL::OpenAL
    PUBLIC_HEADERS
        include/Volcano/Acoustics/Common.h
        include/Volcano/Acoustics/Space.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/Volcano/Acoustics/Config.h
        DESTINATION include/Volcano/Acoustics
)
