
add_executable(server)

add_dependencies(server base)
set_target_properties(server PROPERTIES OUTPUT_NAME "VolcanoServer")

target_include_directories(server
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

target_link_libraries(server
    PRIVATE
        base
    )

target_sources(server
    PRIVATE
        include/Volcano/Server/Common.hpp
        src/Volcano/Server/Main.cpp
    )

get_target_property(SERVER_SOURCES server SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SERVER_SOURCES})
    
