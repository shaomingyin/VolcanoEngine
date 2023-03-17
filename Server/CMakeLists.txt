
volcano_glob_sources(SERVER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR})
volcano_define_executable(Server
    OUTPUT_NAME
        VolcanoServer
    PRIVATE_SOURCES
        ${SERVER_SOURCES}
    PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )
