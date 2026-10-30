#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <limits>
#include <span>
#include <stdexcept>
#include <string>
#include <utility>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <physfs.h>

#include <Volcano/Buffer.h>
#include <Volcano/PhysFile.h>
#include <Volcano/ScopeGuard.h>
#include <Volcano/Stream.h>
#include <Volcano/Schema/Material.h>
#include <Volcano/Schema/Mesh.h>
#include <Volcano/Schema/Model.h>
#include <Volcano/Schema/Texture.h>

namespace {

using Volcano::Buffer;
using Volcano::ByteArray;
using Volcano::Stream;
namespace Schema = Volcano::Schema;
using Json = nlohmann::json;

template<typename Resource>
ByteArray encode(const Resource& resource) {
    Buffer stream;
    stream.open(Buffer::WriteOnly);
    resource.write(stream);
    return stream.data();
}

template<typename Resource>
Resource decode(ByteArray bytes) {
    Buffer stream(std::move(bytes));
    stream.open(Buffer::ReadOnly);
    return Resource::read(stream);
}

ByteArray jsonBytes(const Json& json) {
    const auto text = json.dump();
    return ByteArray(text.begin(), text.end());
}

Schema::Mesh mesh() {
    Schema::Mesh value;
    value.vertices = {{{1, -2, 0.5f}, {0, 0, 1}, {0.25f, 0.75f}}};
    value.indices = {0, 0, 0};
    return value;
}

Schema::Texture texture() {
    return {2, 1, {128, 64, 0, 128, 0, 0, 0, 0}};
}

Schema::Model model() {
    Schema::Model value;
    Schema::Model::Part part;
    part.mesh = std::filesystem::path(u8"meshes/立方体.vemesh");
    part.material = "materials/test.vematerial";
    part.transform.matrix() << 1, 2, 0, 3,
        0, 1, 0, 4,
        0, 0, -1, 5,
        0, 0, 0, 1;
    value.parts.push_back(part);
    value.parts.push_back({part.mesh, part.material, Eigen::Affine3f::Identity()});
    return value;
}

void setU32(ByteArray& bytes, std::size_t offset, std::uint32_t value) {
    for (unsigned index = 0; index < 4; ++index) {
        bytes.at(offset + index) = static_cast<std::uint8_t>(value >> (index * 8));
    }
}

class ShortStream final: public Stream {
public:
    bool isReadable() const override {
        return true;
    }

    bool isWritable() const override {
        return true;
    }

    std::size_t read(std::span<std::uint8_t> data) override {
        if (failRead) {
            throw std::runtime_error("Read failed");
        }
        const auto count = std::min({data.size(), input.size() - position, std::size_t(3)});
        std::copy_n(input.begin() + position, count, data.begin());
        position += count;
        return count;
    }

    std::size_t write(std::span<const std::uint8_t> data) override {
        if (failWrite) {
            throw std::runtime_error("Write failed");
        }
        if (stallWrite) {
            return 0;
        }
        const auto count = std::min(data.size(), std::size_t(3));
        output.insert(output.end(), data.begin(), data.begin() + count);
        return count;
    }

public:
    ByteArray input;
    ByteArray output;
    std::size_t position = 0;
    bool failRead = false;
    bool failWrite = false;
    bool stallWrite = false;
};

template<typename Resource>
void expectShortTransfers(const Resource& resource) {
    ShortStream writer;
    resource.write(writer);
    ShortStream reader;
    reader.input = writer.output;
    const auto decoded = Resource::read(reader);
    EXPECT_EQ(encode(decoded), encode(resource));
    EXPECT_EQ(reader.position, reader.input.size());
}

template<typename Resource>
void expectInvalidWrite(const Resource& resource) {
    ShortStream stream;
    EXPECT_THROW(resource.write(stream), std::invalid_argument);
    EXPECT_TRUE(stream.output.empty());
}

TEST(SchemaModel, RoundTripPreservesPartsPathsAndAffineMatrices) {
    const auto original = model();
    const auto bytes = encode(original);
    const auto decoded = decode<Schema::Model>(bytes);
    ASSERT_EQ(decoded.parts.size(), 2);
    for (std::size_t index = 0; index < original.parts.size(); ++index) {
        EXPECT_EQ(decoded.parts[index].mesh, original.parts[index].mesh);
        EXPECT_EQ(decoded.parts[index].material, original.parts[index].material);
        EXPECT_EQ(decoded.parts[index].transform.matrix(), original.parts[index].transform.matrix());
    }
    const auto document = Json::parse(bytes.begin(), bytes.end());
    EXPECT_EQ(document.at("type"), "model");
    EXPECT_EQ(document.at("version"), 1);
    EXPECT_EQ(document.at("parts").at(0).at("transform"),
        Json({1, 2, 0, 3, 0, 1, 0, 4, 0, 0, -1, 5, 0, 0, 0, 1}));
    EXPECT_TRUE(decode<Schema::Model>(encode(Schema::Model{})).parts.empty());
}

TEST(SchemaModel, RejectsMissingAndMalformedJsonFields) {
    const auto bytes = encode(model());
    const auto valid = Json::parse(bytes.begin(), bytes.end());
    for (const auto& version : {Json(0), Json(2), Json(-1), Json(1.0), Json("1"), Json(nullptr)}) {
        auto document = valid;
        document["version"] = version;
        EXPECT_THROW(decode<Schema::Model>(jsonBytes(document)), std::invalid_argument);
    }
    for (const auto* field : {"type", "version", "parts"}) {
        auto document = valid;
        document.erase(field);
        EXPECT_THROW(decode<Schema::Model>(jsonBytes(document)), std::invalid_argument);
    }
    auto document = valid;
    document["type"] = "material";
    EXPECT_THROW(decode<Schema::Model>(jsonBytes(document)), std::invalid_argument);
    document = valid;
    document["parts"] = Json::object();
    EXPECT_THROW(decode<Schema::Model>(jsonBytes(document)), std::invalid_argument);
    document = valid;
    document["parts"][0]["transform"] = {1, 2};
    EXPECT_THROW(decode<Schema::Model>(jsonBytes(document)), std::invalid_argument);
    document = valid;
    document["parts"][0]["transform"][0] = "1";
    EXPECT_THROW(decode<Schema::Model>(jsonBytes(document)), std::invalid_argument);
    document = valid;
    document["parts"][0].erase("mesh");
    EXPECT_THROW(decode<Schema::Model>(jsonBytes(document)), std::invalid_argument);
    EXPECT_THROW(decode<Schema::Model>(ByteArray{'{'}), std::invalid_argument);
    auto trailing = bytes;
    trailing.push_back('x');
    EXPECT_THROW(decode<Schema::Model>(trailing), std::invalid_argument);
    document = valid;
    document["note"] = "ignored";
    EXPECT_NO_THROW(decode<Schema::Model>(jsonBytes(document)));
}

TEST(SchemaModel, RejectsInvalidTransformsAndPathsBeforeWriting) {
    for (const auto value : {std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::infinity()}) {
        auto resource = model();
        resource.parts[0].transform.matrix()(0, 0) = value;
        expectInvalidWrite(resource);
    }
    auto resource = model();
    resource.parts[0].transform.matrix()(3, 0) = 0.1f;
    expectInvalidWrite(resource);
    for (const auto& path : {std::string(), std::string("/absolute"), std::string("../parent"),
        std::string("mesh/../parent"), std::string("./mesh"), std::string("C:/mesh"),
        std::string("mesh\\name"), std::string("mesh\0name", 9), std::string("\xff", 1)}) {
        auto invalid = model();
        invalid.parts[0].mesh = std::filesystem::path(path);
        expectInvalidWrite(invalid);
    }
    resource = model();
    resource.parts[0].material.clear();
    expectInvalidWrite(resource);
}

TEST(SchemaMaterial, RoundTripPreservesCompleteDescription) {
    Schema::Material original;
    original.albedo = {0.2f, 0.4f, 0.6f, 0.5f};
    original.specular = 0.7f;
    original.metallic = 0.8f;
    original.roughness = 0.3f;
    original.emissive = {3, 2, 1};
    original.albedoTexture = "textures/albedo.vetexture";
    original.normalTexture = "textures/normal.vetexture";
    original.metallicRoughnessTexture = "textures/mr.vetexture";
    original.lit = false;
    original.alphaMode = Schema::Material::AlphaMode::Blend;
    const auto decoded = decode<Schema::Material>(encode(original));
    EXPECT_EQ(decoded.albedo, original.albedo);
    EXPECT_EQ(decoded.specular, original.specular);
    EXPECT_EQ(decoded.metallic, original.metallic);
    EXPECT_EQ(decoded.roughness, original.roughness);
    EXPECT_EQ(decoded.emissive, original.emissive);
    EXPECT_EQ(decoded.albedoTexture, original.albedoTexture);
    EXPECT_EQ(decoded.normalTexture, original.normalTexture);
    EXPECT_EQ(decoded.metallicRoughnessTexture, original.metallicRoughnessTexture);
    EXPECT_EQ(decoded.lit, original.lit);
    EXPECT_EQ(decoded.alphaMode, original.alphaMode);
    EXPECT_NO_THROW(decode<Schema::Material>(encode(Schema::Material{})));
}

TEST(SchemaMaterial, RejectsInvalidColorsCoefficientsModesAndPaths) {
    for (const auto bad : {-1.0f, 1.1f, std::numeric_limits<float>::quiet_NaN(),
        std::numeric_limits<float>::infinity()}) {
        for (int field = 0; field < 4; ++field) {
            Schema::Material resource;
            if (field == 0) {
                resource.albedo[3] = bad;
            } else if (field == 1) {
                resource.specular = bad;
            } else if (field == 2) {
                resource.metallic = bad;
            } else {
                resource.roughness = bad;
            }
            expectInvalidWrite(resource);
        }
    }
    Schema::Material resource;
    resource.emissive[0] = -1;
    expectInvalidWrite(resource);
    resource = {};
    resource.alphaMode = static_cast<Schema::Material::AlphaMode>(99);
    expectInvalidWrite(resource);
    resource = {};
    resource.normalTexture = "../outside";
    expectInvalidWrite(resource);
    resource = {};
    resource.metallicRoughnessTexture = "/absolute";
    expectInvalidWrite(resource);
}

TEST(SchemaMaterial, RejectsIncorrectJsonTypesAndUnsupportedAlphaMode) {
    const auto bytes = encode(Schema::Material{});
    const auto valid = Json::parse(bytes.begin(), bytes.end());
    for (const auto& [key, value] : valid.items()) {
        auto document = valid;
        document.erase(key);
        EXPECT_THROW(decode<Schema::Material>(jsonBytes(document)), std::invalid_argument) << key;
        document = valid;
        document[key] = Json::object();
        EXPECT_THROW(decode<Schema::Material>(jsonBytes(document)), std::invalid_argument) << key;
    }
    auto document = valid;
    document["alphaMode"] = "mask";
    EXPECT_THROW(decode<Schema::Material>(jsonBytes(document)), std::invalid_argument);
    document = valid;
    document["albedo"] = {1, 1, 1};
    EXPECT_THROW(decode<Schema::Material>(jsonBytes(document)), std::invalid_argument);
    document = valid;
    document["roughness"] = 2;
    EXPECT_THROW(decode<Schema::Material>(jsonBytes(document)), std::invalid_argument);
    document = valid;
    document["lit"] = 1;
    EXPECT_THROW(decode<Schema::Material>(jsonBytes(document)), std::invalid_argument);
}

TEST(SchemaMesh, HasPortableLittleEndianLayoutAndRoundTrips) {
    const ByteArray expected = {
        'V', 'E', 'M', 'H', 1, 0, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0,
        0, 0, 128, 63, 0, 0, 0, 192, 0, 0, 0, 63,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63,
        0, 0, 128, 62, 0, 0, 64, 63,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    EXPECT_EQ(encode(mesh()), expected);
    const auto decoded = decode<Schema::Mesh>(expected);
    ASSERT_EQ(decoded.vertices.size(), 1);
    EXPECT_EQ(decoded.vertices[0].position, mesh().vertices[0].position);
    EXPECT_EQ(decoded.vertices[0].normal, mesh().vertices[0].normal);
    EXPECT_EQ(decoded.vertices[0].texcoord, mesh().vertices[0].texcoord);
    EXPECT_EQ(decoded.indices, mesh().indices);
    EXPECT_TRUE(decode<Schema::Mesh>(encode(Schema::Mesh{})).vertices.empty());
}

TEST(SchemaMesh, RejectsBadHeadersLengthsAndCountsBeforeAllocating) {
    const auto valid = encode(mesh());
    for (std::size_t length = 0; length < valid.size(); ++length) {
        EXPECT_THROW(decode<Schema::Mesh>(ByteArray(valid.begin(), valid.begin() + length)), std::invalid_argument);
    }
    auto bytes = valid;
    bytes.push_back(0);
    EXPECT_THROW(decode<Schema::Mesh>(bytes), std::invalid_argument);
    bytes = valid;
    bytes[0] = 'X';
    EXPECT_THROW(decode<Schema::Mesh>(bytes), std::invalid_argument);
    for (const auto offset : {4, 8, 12}) {
        bytes = valid;
        setU32(bytes, offset, 0xffffffff);
        EXPECT_THROW(decode<Schema::Mesh>(bytes), std::invalid_argument);
    }
    bytes = valid;
    setU32(bytes, 12, 2);
    bytes.resize(bytes.size() - 4);
    EXPECT_THROW(decode<Schema::Mesh>(bytes), std::invalid_argument);
}

TEST(SchemaMesh, RejectsNonfiniteAttributesAndOutOfRangeIndices) {
    for (const auto bits : {0x7fc00000u, 0x7f800000u, 0xff800000u}) {
        for (const auto offset : {16, 28, 40}) {
            auto bytes = encode(mesh());
            setU32(bytes, offset, bits);
            EXPECT_THROW(decode<Schema::Mesh>(bytes), std::invalid_argument);
        }
    }
    auto bytes = encode(mesh());
    setU32(bytes, 48, 1);
    EXPECT_THROW(decode<Schema::Mesh>(bytes), std::invalid_argument);
    auto resource = mesh();
    resource.indices = {0, 0};
    expectInvalidWrite(resource);
    resource = mesh();
    resource.vertices[0].texcoord[0] = std::numeric_limits<float>::quiet_NaN();
    expectInvalidWrite(resource);
    resource = mesh();
    resource.indices[0] = 1;
    expectInvalidWrite(resource);
    resource = mesh();
    resource.vertices[0].texcoord = {-2, 4};
    EXPECT_NO_THROW(resource.validate());
}

TEST(SchemaTexture, HasPortableHeaderAndPreservesPremultipliedPixelBytes) {
    const ByteArray expected = {'V', 'E', 'T', 'X', 1, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0,
        128, 64, 0, 128, 0, 0, 0, 0};
    EXPECT_EQ(encode(texture()), expected);
    const auto decoded = decode<Schema::Texture>(expected);
    EXPECT_EQ(decoded.width, 2);
    EXPECT_EQ(decoded.height, 1);
    EXPECT_EQ(decoded.pixels, texture().pixels);
}

TEST(SchemaTexture, RejectsTruncationExtraDataVersionsAndOverflowingDimensions) {
    const auto valid = encode(texture());
    for (std::size_t length = 0; length < valid.size(); ++length) {
        EXPECT_THROW(decode<Schema::Texture>(ByteArray(valid.begin(), valid.begin() + length)), std::invalid_argument);
    }
    auto bytes = valid;
    bytes.push_back(0);
    EXPECT_THROW(decode<Schema::Texture>(bytes), std::invalid_argument);
    bytes = valid;
    bytes[0] = 'X';
    EXPECT_THROW(decode<Schema::Texture>(bytes), std::invalid_argument);
    bytes = valid;
    setU32(bytes, 4, 2);
    EXPECT_THROW(decode<Schema::Texture>(bytes), std::invalid_argument);
    bytes = valid;
    setU32(bytes, 8, 0xffffffff);
    setU32(bytes, 12, 0xffffffff);
    EXPECT_THROW(decode<Schema::Texture>(bytes), std::invalid_argument);
    bytes = valid;
    setU32(bytes, 8, 0);
    bytes.resize(16);
    EXPECT_THROW(decode<Schema::Texture>(bytes), std::invalid_argument);
}

TEST(SchemaTexture, RejectsWrongPixelCountsAndStraightAlphaWithoutConverting) {
    expectInvalidWrite(Schema::Texture{});
    auto resource = texture();
    resource.pixels.pop_back();
    expectInvalidWrite(resource);
    resource = texture();
    resource.pixels[0] = 129;
    expectInvalidWrite(resource);
    auto bytes = encode(texture());
    bytes[16] = 129;
    EXPECT_THROW(decode<Schema::Texture>(bytes), std::invalid_argument);
}

TEST(SchemaStreams, SupportsShortTransfersOnSequentialStreams) {
    expectShortTransfers(model());
    expectShortTransfers(Schema::Material{});
    expectShortTransfers(mesh());
    expectShortTransfers(texture());
}

TEST(SchemaStreams, StartsAtCurrentPositionAndConsumesRemainingResource) {
    const auto check = []<typename Resource>(const Resource& resource) {
        Buffer buffer({1, 2, 3});
        buffer.open(Buffer::ReadWrite);
        buffer.seek(3);
        resource.write(buffer);
        EXPECT_EQ(buffer.data()[0], 1);
        buffer.seek(3);
        const auto decoded = Resource::read(buffer);
        EXPECT_EQ(encode(decoded), encode(resource));
        EXPECT_EQ(buffer.tell(), buffer.size());
    };
    check(model());
    check(Schema::Material{});
    check(mesh());
    check(texture());
}

TEST(SchemaStreams, PropagatesIoErrorsAndRejectsStalledWrites) {
    const auto check = []<typename Resource>(const Resource& resource) {
        ShortStream input;
        input.failRead = true;
        EXPECT_THROW(Resource::read(input), std::runtime_error);
        ShortStream output;
        output.failWrite = true;
        EXPECT_THROW(resource.write(output), std::runtime_error);
        output.failWrite = false;
        output.stallWrite = true;
        EXPECT_THROW(resource.write(output), std::runtime_error);
    };
    check(model());
    check(Schema::Material{});
    check(mesh());
    check(texture());
}

TEST(SchemaStreams, ReadsAndWritesThroughPhysFileWithoutOwningIt) {
    const auto root = std::filesystem::temp_directory_path() /
        ("volcano-schema-" + std::to_string(Volcano::Clock::now().time_since_epoch().count()));
    std::filesystem::create_directories(root);
    auto directoryGuard = Volcano::scopeGuard([&] {
        std::error_code error;
        std::filesystem::remove_all(root, error);
    });
    ASSERT_NE(PHYSFS_init("SchemaCheck"), 0);
    auto physfsGuard = Volcano::scopeGuard([] {
        EXPECT_NE(PHYSFS_deinit(), 0);
    });
    const auto path = root.u8string();
    const std::string native(path.begin(), path.end());
    ASSERT_NE(PHYSFS_mount(native.c_str(), nullptr, 1), 0);
    ASSERT_NE(PHYSFS_setWriteDir(native.c_str()), 0);
    Volcano::PhysFile output("mesh.vemesh", Volcano::PhysFile::OpenMode::WriteOnly);
    mesh().write(output);
    EXPECT_TRUE(output.isOpen());
    output.close();
    Volcano::PhysFile input("mesh.vemesh");
    const auto decoded = Schema::Mesh::read(input);
    EXPECT_EQ(encode(decoded), encode(mesh()));
    EXPECT_TRUE(input.isOpen());
}

} // namespace
