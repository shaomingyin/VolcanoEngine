
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
    include/Volcano/PhysFile.h
    include/Volcano/ScopeGuard.h
    include/Volcano/Version.h
    src/Volcano/Common.cpp
    src/Volcano/Error.cpp
    src/Volcano/PhysFile.cpp
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
    NOMINMAX
)

if(MSVC)
    target_compile_options(Base PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:/GR->")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(Base PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>")
endif()

volcano_target_setup(Base)
set_target_properties(Base PROPERTIES OUTPUT_NAME VolcanoBase)

find_package(Threads REQUIRED)
target_link_libraries(Base PUBLIC Threads::Threads)

find_package(Async++ CONFIG REQUIRED)
target_link_libraries(Base PUBLIC Async++)

find_package(PhysFS CONFIG REQUIRED)
target_link_libraries(Base PUBLIC physfs-static)

find_package(protobuf CONFIG REQUIRED)
target_link_libraries(Base PUBLIC protobuf::protobuf)

find_package(Eigen3 CONFIG REQUIRED)
target_link_libraries(Base PUBLIC Eigen3::Eigen)

find_package(spdlog REQUIRED)
target_link_libraries(Base PUBLIC spdlog::spdlog)

if(BUILD_TESTING)
    foreach(test IN ITEMS Cache PhysFile Version Utility Color Math Transform)
        add_executable(Base${test}Check tests/${test}.cpp)
        target_link_libraries(Base${test}Check PRIVATE Base GTest::gtest_main)
        gtest_discover_tests(Base${test}Check)
    endforeach()
endif()
