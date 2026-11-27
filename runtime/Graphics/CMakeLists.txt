
add_library(Graphics STATIC
    include/Volcano/Graphics/Buffer.h
    include/Volcano/Graphics/BufferAllocator.h
    include/Volcano/Graphics/Common.h
    include/Volcano/Graphics/Config.h.in
    include/Volcano/Graphics/Context.h
    include/Volcano/Graphics/Drawable.h
    include/Volcano/Graphics/Model.h
    include/Volcano/Graphics/Renderer.h
    include/Volcano/Graphics/Pipeline.h
    include/Volcano/Graphics/View.h
    include/Volcano/Graphics/VirtualTexture.h
    src/Volcano/Graphics/Buffer.cpp
    src/Volcano/Graphics/BufferImpl.cpp
    src/Volcano/Graphics/BufferImpl.h
    src/Volcano/Graphics/BufferAllocator.cpp
    src/Volcano/Graphics/Common.cpp
    src/Volcano/Graphics/Context.cpp
    src/Volcano/Graphics/Drawable.cpp
    src/Volcano/Graphics/Model.cpp
    src/Volcano/Graphics/Renderer.cpp
    src/Volcano/Graphics/Pipeline.cpp
    src/Volcano/Graphics/View.cpp
    src/Volcano/Graphics/VirtualTexture.cpp
)

configure_file(
    include/Volcano/Graphics/Config.h.in
    include/Volcano/Graphics/Config.h
)

set_target_properties(Graphics PROPERTIES OUTPUT_NAME VolcanoGraphics)

target_include_directories(Graphics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

target_link_libraries(Graphics PUBLIC Base PRIVATE World)

volcano_link_qt(Graphics OpenGL)
volcano_setup_target(Graphics)

if(BUILD_TESTING)
    find_package(Qt6 REQUIRED COMPONENTS Quick)
    find_package(GTest CONFIG REQUIRED)
    add_executable(GraphicsTests tests/RendererTest.cpp)
    target_link_libraries(GraphicsTests PRIVATE Graphics World Qt6::Quick GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(GraphicsTests)
endif()
