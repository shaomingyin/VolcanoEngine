add_library(Physics STATIC
    include/Volcano/Physics/Common.h
    include/Volcano/Physics/System.h
    src/Volcano/Physics/System.cpp
)

find_package(Qt6 REQUIRED COMPONENTS Gui)
target_include_directories(Physics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
find_package(Jolt REQUIRED)
target_link_libraries(Physics PUBLIC Base World Qt6::Gui Jolt::Jolt)
volcano_setup_target(Physics)
set_target_properties(Physics PROPERTIES OUTPUT_NAME VolcanoPhysics)

if(BUILD_TESTING)
    find_package(Qt6 REQUIRED COMPONENTS Qml)
    find_package(GTest CONFIG REQUIRED)
    add_executable(PhysicsTests tests/SystemTest.cpp)
    target_link_libraries(PhysicsTests PRIVATE Physics Qt6::Qml GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(PhysicsTests)
endif()
