
find_package(Qt6 REQUIRED Qml)

file(GLOB_RECURSE VOLCANO_BASE_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_library(base STATIC ${VOLCANO_BASE_SOURCES})
set_target_properties(base PROPERTIES OUTPUT_NAME "VolcanoBase")
target_compile_features(base PUBLIC cxx_std_17)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VOLCANO_BASE_SOURCES})

target_include_directories(base
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

target_link_libraries(base
    PUBLIC
        Qt6::Qml
    )

configure_file(
    include/Volcano/Config.hpp.in
    include/Volcano/Config.hpp
    )
