
add_library(World STATIC
    include/Volcano/World/Common.h
    include/Volcano/World/Config.h.in
    include/Volcano/World/Inherent.h
    include/Volcano/World/Scene.h
    src/Volcano/World/Common.cpp
    src/Volcano/World/Inherent.cpp
    src/Volcano/World/Scene.cpp
)

configure_file(
    include/Volcano/World/Config.h.in
    include/Volcano/World/Config.h
)

target_include_directories(World PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(World)

set_target_properties(World PROPERTIES OUTPUT_NAME VolcanoWorld)

target_link_libraries(World PUBLIC Base)

find_package(EnTT REQUIRED)
target_link_libraries(World PUBLIC EnTT::EnTT)

#set(SCHEMA_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen)
#find_package(protobuf)
#protobuf_generate(
#    TARGET World
#    PROTOC_OUT_DIR ${SCHEMA_GEN_DIR}/Volcano/World
#    OUT_VAR SCHEMA_SOURCES
#)
#source_group(\\gen FILES ${SCHEMA_SOURCES})
#target_include_directories(World PUBLIC ${SCHEMA_GEN_DIR})
