add_library(Physics STATIC
    include/Volcano/Physics/Common.h
    include/Volcano/Physics/System.h
    src/Volcano/Physics/System.cpp
)
target_include_directories(Physics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
find_package(Jolt REQUIRED)
target_link_libraries(Physics PUBLIC World Jolt::Jolt)
volcano_target_setup(Physics)
set_target_properties(Physics PROPERTIES OUTPUT_NAME VolcanoPhysics)

if(BUILD_TESTING)
    add_executable(PhysicsCheck tests/System.cpp)
    target_link_libraries(PhysicsCheck PRIVATE Physics GTest::gtest_main)
    gtest_discover_tests(PhysicsCheck)
endif()
