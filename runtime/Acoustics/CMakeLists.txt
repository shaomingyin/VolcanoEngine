
add_library(Acoustics STATIC
    include/Volcano/Acoustics/Config.h.in
    include/Volcano/Acoustics/Common.h
    include/Volcano/Acoustics/Space.h
    src/Volcano/Acoustics/Space.cpp
)

configure_file(
    include/Volcano/Acoustics/Config.h.in
    include/Volcano/Acoustics/Config.h
)

set_target_properties(Acoustics PROPERTIES OUTPUT_NAME VolcanoAcoustics)

target_include_directories(Acoustics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

target_link_libraries(Acoustics PUBLIC Base)

volcano_link_package(Acoustics OpenAL OpenAL::OpenAL)
volcano_setup_target(Acoustics)
