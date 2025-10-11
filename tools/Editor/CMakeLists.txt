
add_executable(Editor
    include/Volcano/Editor/Common.h
    include/Volcano/Editor/Config.h.in
    src/Volcano/Editor/Main.cpp
)

configure_file(
    include/Volcano/Editor/Config.h.in
    include/Volcano/Editor/Config.h
)

set_target_properties(Editor PROPERTIES OUTPUT_NAME VolcanoEditor)

target_include_directories(Editor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

target_link_libraries(Editor World Graphics Gui)

find_package(SFML REQUIRED COMPONENTS sfml-window)
target_link_libraries(Editor sfml-window)

