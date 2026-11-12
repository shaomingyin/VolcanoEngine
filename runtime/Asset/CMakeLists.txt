add_library(Asset STATIC
    include/Volcano/Asset/Common.h
    include/Volcano/Asset/Mesh.h
    include/Volcano/Asset/Texture.h
    src/Volcano/Asset/Format.h
    src/Volcano/Asset/Mesh.cpp
    src/Volcano/Asset/Texture.cpp
)

volcano_target_setup(Asset)
set_target_properties(Asset PROPERTIES OUTPUT_NAME VolcanoAsset)

find_package(protobuf CONFIG REQUIRED)
if(protobuf_PACKAGE_FOLDER_DEBUG AND EXISTS "${protobuf_PACKAGE_FOLDER_DEBUG}/bin/protoc")
    set_property(TARGET protobuf::protoc PROPERTY IMPORTED_LOCATION "${protobuf_PACKAGE_FOLDER_DEBUG}/bin/protoc")
elseif(protobuf_PACKAGE_FOLDER_RELEASE AND EXISTS "${protobuf_PACKAGE_FOLDER_RELEASE}/bin/protoc")
    set_property(TARGET protobuf::protoc PROPERTY IMPORTED_LOCATION "${protobuf_PACKAGE_FOLDER_RELEASE}/bin/protoc")
elseif(protobuf_PACKAGE_FOLDER AND EXISTS "${protobuf_PACKAGE_FOLDER}/bin/protoc")
    set_property(TARGET protobuf::protoc PROPERTY IMPORTED_LOCATION "${protobuf_PACKAGE_FOLDER}/bin/protoc")
endif()

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated)
protobuf_generate(
    TARGET Asset
    LANGUAGE cpp
    PROTOS Schema.proto
    IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
    PROTOC_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated
)

target_include_directories(Asset PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/generated
)

target_link_libraries(Asset PUBLIC Base)

if(BUILD_TESTING)
    add_executable(AssetCheck tests/Resources.cpp)
    target_link_libraries(AssetCheck PRIVATE Asset GTest::gtest_main)
    gtest_discover_tests(AssetCheck)
endif()
