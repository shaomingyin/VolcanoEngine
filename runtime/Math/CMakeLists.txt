add_library(Math INTERFACE
    include/Volcano/Math/Common.h
    include/Volcano/Math/Math.h
    include/Volcano/Math/Vector.h
    include/Volcano/Math/Matrix.h
    include/Volcano/Math/Quaternion.h
    include/Volcano/Math/BoundingBox.h
    include/Volcano/Math/Rect.h
    include/Volcano/Math/Affine.h
    include/Volcano/Math/Color.h
)

target_include_directories(Math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Math INTERFACE Base)
volcano_target_setup(Math)

if(BUILD_TESTING)
    add_executable(MathCheck tests/Math.cpp)
    target_link_libraries(MathCheck PRIVATE Math GTest::gtest_main)
    gtest_discover_tests(MathCheck)

    foreach(test IN ITEMS Geometry Color)
        add_executable(Math${test}Check tests/${test}.cpp)
        target_link_libraries(Math${test}Check PRIVATE Math GTest::gtest_main)
        gtest_discover_tests(Math${test}Check)
    endforeach()
endif()
