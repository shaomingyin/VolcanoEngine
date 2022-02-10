
link_libraries(Runtime)
link_qt_modules(Network)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    )

file(GLOB_RECURSE SERVER_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.in
    )

qt_add_executable(Server ${SERVER_SOURCES})
set_target_properties(Server PROPERTIES OUTPUT_NAME "VolcanoServer")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SERVER_SOURCES})

configure_file(
    include/Volcano/Server/Config.hpp.in
    include/Volcano/Server/Config.hpp
    )
