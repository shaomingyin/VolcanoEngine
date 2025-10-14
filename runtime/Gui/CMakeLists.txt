
add_library(Gui STATIC
    include/Volcano/Gui/Common.h
    include/Volcano/Gui/Config.h.in
    include/Volcano/Gui/Layout.h
    include/Volcano/Gui/Widget.h
    include/Volcano/Gui/Window.h
    src/Volcano/Gui/Common.cpp
    src/Volcano/Gui/Layout.cpp
    src/Volcano/Gui/Widget.cpp
    src/Volcano/Gui/Window.cpp
)

configure_file(
    include/Volcano/Gui/Config.h.in
    include/Volcano/Gui/Config.h
)

target_include_directories(Gui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(Gui)
set_target_properties(Gui PROPERTIES OUTPUT_NAME VolcanoGui)

target_link_libraries(Gui PUBLIC Base)

find_package(SFML REQUIRED COMPONENTS sfml-audio sfml-graphics)
target_link_libraries(Gui PUBLIC sfml-audio sfml-graphics)
