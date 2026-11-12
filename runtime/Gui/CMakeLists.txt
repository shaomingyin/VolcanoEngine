
add_library(Gui STATIC
    include/Volcano/Gui/Style.h
    include/Volcano/Gui/Widget.h
    include/Volcano/Gui/Layout.h
    include/Volcano/Gui/Text.h
    include/Volcano/Gui/Image.h
    include/Volcano/Gui/CheckBox.h
    include/Volcano/Gui/Slider.h
    include/Volcano/Gui/ProgressBar.h
    include/Volcano/Gui/Button.h
    include/Volcano/Gui/Event.h
    include/Volcano/Gui/Document.h
    include/layout.h
    src/Volcano/Gui/Widget.cpp
    src/Volcano/Gui/Layout.cpp
    src/Volcano/Gui/Text.cpp
    src/Volcano/Gui/Image.cpp
    src/Volcano/Gui/CheckBox.cpp
    src/Volcano/Gui/Slider.cpp
    src/Volcano/Gui/ProgressBar.cpp
    src/Volcano/Gui/Button.cpp
    src/Volcano/Gui/Document.cpp
    include/Volcano/Gui/Common.h
    include/Volcano/Gui/Config.h.in
    include/Volcano/Gui/Platform.h
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

target_link_libraries(Gui PUBLIC Base World)
target_compile_definitions(Gui PUBLIC LAY_FLOAT=1)

if(BUILD_TESTING)
    add_executable(GuiCheck tests/Document.cpp tests/Layout.cpp tests/Tree.cpp tests/Widgets.cpp tests/Platform.h)
    target_link_libraries(GuiCheck PRIVATE Gui GTest::gtest_main)
    gtest_discover_tests(GuiCheck)
endif()
