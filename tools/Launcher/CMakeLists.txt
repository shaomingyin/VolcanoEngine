
add_executable(Launcher
    include/Volcano/Launcher/Client.h
    include/Volcano/Launcher/Common.h
    include/Volcano/Launcher/Config.h.in
    include/Volcano/Launcher/Console.h
    include/Volcano/Launcher/Local.h
    include/Volcano/Launcher/Window.h
    src/Volcano/Launcher/Client.cpp
    src/Volcano/Launcher/Console.cpp
    src/Volcano/Launcher/Local.cpp
    src/Volcano/Launcher/Main.cpp
    src/Volcano/Launcher/Window.cpp
)

configure_file(
    include/Volcano/Launcher/Config.h.in
    include/Volcano/Launcher/Config.h
)

set_target_properties(Launcher PROPERTIES OUTPUT_NAME VolcanoLauncher)

target_include_directories(Launcher PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(Launcher)
target_link_libraries(Launcher PRIVATE "$<LINK_LIBRARY:WHOLE_ARCHIVE,Gameplay>")
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(Launcher PRIVATE Gui Graphics glfw)

find_package(argh REQUIRED)
target_link_libraries(Launcher PUBLIC argh)

if(BUILD_TESTING)
    add_executable(LauncherWindowCheck tests/Window.cpp src/Volcano/Launcher/Window.cpp)
    target_include_directories(LauncherWindowCheck PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )
    target_link_libraries(LauncherWindowCheck PRIVATE Graphics glfw GTest::gtest_main)
    gtest_discover_tests(LauncherWindowCheck PROPERTIES LABELS display TIMEOUT 30)
endif()
