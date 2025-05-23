
volcano_add_static_library(Framework
    OUTPUT_NAME
        VolcanoFramework
    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE_SOURCES
        include/Volcano/Framework/Application.h
        include/Volcano/Framework/Base.h
        include/Volcano/Framework/Client.h
        include/Volcano/Framework/Config.h.in
        include/Volcano/Framework/Context.h
        include/Volcano/Framework/Common.h
        include/Volcano/Framework/Local.h
        include/Volcano/Framework/Server.h
        src/Volcano/Framework/Application.cpp
        src/Volcano/Framework/Base.cpp
        src/Volcano/Framework/Client.cpp
        src/Volcano/Framework/Context.cpp
        src/Volcano/Framework/Local.cpp
        src/Volcano/Framework/Server.cpp
    PUBLIC_LINK_LIBRARIES
        Physics
        Gui
    PUBLIC_LINK_PACKAGES
        ada ada::ada
)
