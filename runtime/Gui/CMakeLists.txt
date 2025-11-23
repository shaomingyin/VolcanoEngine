
add_library(Gui STATIC
    include/Volcano/Gui/Common.hpp
    include/Volcano/Gui/Config.hpp.in
    include/Volcano/Gui/Context.hpp
    include/Volcano/Gui/Layout.hpp
    include/Volcano/Gui/Object.hpp
    include/Volcano/Gui/Style.hpp
    include/Volcano/Gui/Widget.hpp
    include/Volcano/Gui/Window.hpp
    src/Volcano/Gui/Context.cpp
    src/Volcano/Gui/Layout.cpp
    src/Volcano/Gui/Object.cpp
    src/Volcano/Gui/Widget.cpp
    src/Volcano/Gui/Window.cpp
)

configure_file(
    include/Volcano/Gui/Config.hpp.in
    include/Volcano/Gui/Config.hpp
)

target_include_directories(Gui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(Gui)
set_target_properties(Gui PROPERTIES OUTPUT_NAME VolcanoGui)

target_link_libraries(Gui PUBLIC Game)

find_package(SFML REQUIRED COMPONENTS sfml-graphics sfml-window)
target_link_libraries(Gui PUBLIC sfml-graphics sfml-window)
