add_library(Framework STATIC
    include/Volcano/Framework/Common.h
    include/Volcano/Framework/Base.h
    include/Volcano/Framework/Server.h
    src/Volcano/Framework/Resource.h
    src/Volcano/Framework/Base.cpp
    src/Volcano/Framework/Server.cpp
)
target_include_directories(Framework PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Framework PUBLIC World Schema)
volcano_target_setup(Framework)
set_target_properties(Framework PROPERTIES OUTPUT_NAME VolcanoFramework)

add_library(FrameworkLocal STATIC
    include/Volcano/Framework/Local.h
    include/Volcano/Framework/Client.h
    include/Volcano/Framework/Window.h
    src/Volcano/Framework/Local.cpp
    src/Volcano/Framework/Client.cpp
    src/Volcano/Framework/Window.cpp
)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(FrameworkLocal PUBLIC Framework Graphics glfw)
volcano_target_setup(FrameworkLocal)
set_target_properties(FrameworkLocal PROPERTIES OUTPUT_NAME VolcanoFrameworkLocal)

if(BUILD_TESTING)
    add_executable(FrameworkCheck tests/Lifecycle.cpp)
    target_link_libraries(FrameworkCheck PRIVATE Framework GTest::gtest_main)
    gtest_discover_tests(FrameworkCheck)

    add_executable(FrameworkLocalCheck tests/Local.cpp)
    target_link_libraries(FrameworkLocalCheck PRIVATE FrameworkLocal GTest::gtest_main)
    gtest_discover_tests(FrameworkLocalCheck PROPERTIES LABELS display TIMEOUT 30)

    add_executable(FrameworkWindowCheck tests/Window.cpp tests/Lighting.cpp)
    target_link_libraries(FrameworkWindowCheck PRIVATE FrameworkLocal GTest::gtest_main)
    gtest_discover_tests(FrameworkWindowCheck PROPERTIES LABELS display TIMEOUT 30)
endif()
