
add_library(World STATIC
    include/Volcano/World/Texture.h
    include/Volcano/World/Hud.h
    include/Volcano/World/Common.h
    include/Volcano/World/Config.h.in
    include/Volcano/World/Inherent.h
    include/Volcano/World/Scene.h
    include/Volcano/World/Camera.h
    include/Volcano/World/DirectionalLight.h
    include/Volcano/World/Light.h
    include/Volcano/World/Model.h
    include/Volcano/World/RigidBody.h
    include/Volcano/World/AudioSource.h
    include/Volcano/World/AudioListener.h
    include/Volcano/World/OrthographicCamera.h
    include/Volcano/World/PerspectiveCamera.h
    include/Volcano/World/PointLight.h
    include/Volcano/World/SpotLight.h
    src/Volcano/World/RigidBody.cpp
    src/Volcano/World/Camera.cpp
    src/Volcano/World/DirectionalLight.cpp
    src/Volcano/World/Light.cpp
    src/Volcano/World/OrthographicCamera.cpp
    src/Volcano/World/PerspectiveCamera.cpp
    src/Volcano/World/PointLight.cpp
    src/Volcano/World/SpotLight.cpp
    src/Volcano/World/Common.cpp
    src/Volcano/World/Scene.cpp
)

configure_file(
    include/Volcano/World/Config.h.in
    include/Volcano/World/Config.h
)

target_include_directories(World PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(World)

set_target_properties(World PROPERTIES OUTPUT_NAME VolcanoWorld)

target_link_libraries(World PUBLIC Base Gui)

find_package(EnTT REQUIRED)
target_link_libraries(World PUBLIC EnTT::EnTT)


#set(SCHEMA_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen)
#find_package(protobuf)
#protobuf_generate(
#    TARGET World
#    PROTOC_OUT_DIR ${SCHEMA_GEN_DIR}/Volcano/World
#    OUT_VAR SCHEMA_SOURCES
#)
#source_group(\\gen FILES ${SCHEMA_SOURCES})
#target_include_directories(World PUBLIC ${SCHEMA_GEN_DIR})

if(BUILD_TESTING)
    add_executable(WorldMathCheck tests/Math.cpp tests/Components.cpp)
    target_link_libraries(WorldMathCheck PRIVATE World GTest::gtest_main)
    gtest_discover_tests(WorldMathCheck)
endif()
