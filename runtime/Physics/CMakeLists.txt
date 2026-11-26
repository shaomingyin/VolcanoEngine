add_library(Physics STATIC
    include/Volcano/Physics/Common.h
    include/Volcano/Physics/BoxCollisionShape.h
    include/Volcano/Physics/CapsuleCollisionShape.h
    include/Volcano/Physics/CollisionShape.h
    include/Volcano/Physics/RigidBody.h
    include/Volcano/Physics/SphereCollisionShape.h
    include/Volcano/Physics/System.h
    src/Volcano/Physics/BoxCollisionShape.cpp
    src/Volcano/Physics/CapsuleCollisionShape.cpp
    src/Volcano/Physics/CollisionShape.cpp
    src/Volcano/Physics/RigidBody.cpp
    src/Volcano/Physics/SphereCollisionShape.cpp
    src/Volcano/Physics/System.cpp
)

find_package(Qt6 REQUIRED COMPONENTS Gui Qml)
target_include_directories(Physics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Physics PUBLIC Base Qt6::Gui PRIVATE Qt6::Qml)
volcano_link_package(Physics Jolt Jolt::Jolt)
volcano_setup_target(Physics)
set_target_properties(Physics PROPERTIES OUTPUT_NAME VolcanoPhysics)

if(BUILD_TESTING)
    find_package(GTest CONFIG REQUIRED)
    add_executable(PhysicsTests tests/SystemTest.cpp)
    target_link_libraries(PhysicsTests PRIVATE Physics World Qt6::Qml GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(PhysicsTests)
endif()
