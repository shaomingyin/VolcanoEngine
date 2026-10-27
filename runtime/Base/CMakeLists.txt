
add_library(Base STATIC
    include/Volcano/Cache.h
    include/Volcano/Color.h
    include/Volcano/Rect.h
    include/Volcano/Size.h
    include/Volcano/Transform.h
    include/Volcano/Math.h
    include/Volcano/Common.h
    include/Volcano/Config.h.in
    include/Volcano/Error.h
    include/Volcano/ByteArray.h
    include/Volcano/Buffer.h
    include/Volcano/PhysFile.h
    include/Volcano/Stream.h
    include/Volcano/ScopeGuard.h
    include/Volcano/Version.h
    src/Volcano/Cache.cpp
    src/Volcano/Common.cpp
    src/Volcano/Error.cpp
    src/Volcano/PhysFile.cpp
    src/Volcano/Stream.cpp
    src/Volcano/Buffer.cpp
    src/Volcano/Transform.cpp
    src/Volcano/Version.cpp
)

configure_file(
    include/Volcano/Config.h.in
    include/Volcano/Config.h
)

target_include_directories(Base PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

target_compile_definitions(Base PUBLIC
    RTTR_STATIC
    NOMINMAX
)

volcano_target_setup(Base)
set_target_properties(Base PROPERTIES OUTPUT_NAME VolcanoBase)

find_package(PhysFS CONFIG REQUIRED)
target_link_libraries(Base PUBLIC physfs-static)

find_package(Eigen3 CONFIG REQUIRED)
target_link_libraries(Base PUBLIC Eigen3::Eigen)

find_package(spdlog REQUIRED)
target_link_libraries(Base PUBLIC spdlog::spdlog)

find_package(Async++ REQUIRED)
target_link_libraries(Base PUBLIC Async++)

if(BUILD_TESTING)
    foreach(test IN ITEMS PhysFile Stream Buffer Version Utility Color Math Transform)
        add_executable(Base${test}Check tests/${test}.cpp)
        target_link_libraries(Base${test}Check PRIVATE Base GTest::gtest_main)
        gtest_discover_tests(Base${test}Check)
    endforeach()
endif()
