
add_library(Base STATIC
    include/buddy2.h
    src/buddy2/buddy2.c
    include/Volcano/Config.h.in
    include/Volcano/Common.h
    include/Volcano/Config.h.in
    src/Volcano/Common.cpp
)

configure_file(
    include/Volcano/Config.h.in
    include/Volcano/Config.h
)

set_target_properties(Base PROPERTIES OUTPUT_NAME VolcanoBase)

target_include_directories(Base PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

target_compile_definitions(Base PUBLIC
    QT_NO_CONTEXTLESS_CONNECT
)

volcano_link_qt(Base Concurrent)
volcano_link_package(Base microprofile microprofile::microprofile)
volcano_setup_target(Base)
