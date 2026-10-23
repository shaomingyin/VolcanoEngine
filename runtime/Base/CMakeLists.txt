
add_library(Base STATIC
    include/Volcano/Cache.h
    include/Volcano/Common.h
    include/Volcano/Config.h.in
    include/Volcano/Error.h
    include/Volcano/FileSystem.h
    include/Volcano/ByteArray.h
    include/Volcano/File.h
    include/Volcano/Stream.h
    include/Volcano/DirEntry.h
    include/Volcano/NativeFileSystem.h
    include/Volcano/VirtualFileSystem.h
    include/Volcano/ScopeGuard.h
    include/Volcano/Version.h
    src/Volcano/Cache.cpp
    src/Volcano/Common.cpp
    src/Volcano/Error.cpp
    src/Volcano/FileSystem.cpp
    src/Volcano/Stream.cpp
    src/Volcano/NativeFileSystem.cpp
    src/Volcano/VirtualFileSystem.cpp
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

find_package(spdlog REQUIRED)
target_link_libraries(Base PUBLIC spdlog::spdlog)

find_package(Async++ REQUIRED)
target_link_libraries(Base PUBLIC Async++)

if(BUILD_TESTING)
    foreach(test IN ITEMS FileSystem Stream Version Utility)
        add_executable(Base${test}Check tests/${test}.cpp)
        target_link_libraries(Base${test}Check PRIVATE Base GTest::gtest_main)
        gtest_discover_tests(Base${test}Check)
    endforeach()
endif()
