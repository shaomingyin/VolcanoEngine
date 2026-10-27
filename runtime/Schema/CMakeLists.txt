add_library(Schema STATIC
    include/Volcano/Schema/Common.h
    include/Volcano/Schema/Model.h
    include/Volcano/Schema/Mesh.h
    include/Volcano/Schema/Material.h
    include/Volcano/Schema/Texture.h
    src/Volcano/Schema/Format.h
    src/Volcano/Schema/Model.cpp
    src/Volcano/Schema/Mesh.cpp
    src/Volcano/Schema/Material.cpp
    src/Volcano/Schema/Texture.cpp
)

target_include_directories(Schema PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
volcano_target_setup(Schema)
set_target_properties(Schema PROPERTIES OUTPUT_NAME VolcanoSchema)

target_link_libraries(Schema PUBLIC Base)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(Schema PRIVATE nlohmann_json::nlohmann_json)

if(BUILD_TESTING)
    add_executable(SchemaCheck tests/Resources.cpp)
    target_link_libraries(SchemaCheck PRIVATE Schema GTest::gtest_main nlohmann_json::nlohmann_json)
    gtest_discover_tests(SchemaCheck)
endif()
