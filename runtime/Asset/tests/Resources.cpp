#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <limits>
#include <span>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <gtest/gtest.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <physfs.h>

#include <Volcano/PhysFile.h>
#include <Volcano/Asset/Common.h>
#include <Volcano/Asset/Mesh.h>
#include <Volcano/Asset/Texture.h>

namespace {

namespace Asset = Volcano::Asset;

class AssetTestFixture : public testing::Test {
protected:
    void SetUp() override {
        const auto now = std::chrono::steady_clock::now().time_since_epoch().count();
        tempDir_ = std::filesystem::temp_directory_path() / ("volcano-asset-test-" + std::to_string(now));
        std::filesystem::create_directories(tempDir_);
        const auto tempU8 = tempDir_.u8string();
        tempPath_ = std::string(tempU8.begin(), tempU8.end());

        ASSERT_NE(PHYSFS_init("AssetCheck"), 0);
        ASSERT_NE(PHYSFS_setWriteDir(tempPath_.c_str()), 0);
        ASSERT_NE(PHYSFS_mount(tempPath_.c_str(), nullptr, 1), 0);
    }

    void TearDown() override {
        if (PHYSFS_isInit() != 0) {
            PHYSFS_deinit();
        }
        std::error_code ec;
        std::filesystem::remove_all(tempDir_, ec);
    }

    static void writeHelper(google::protobuf::io::ZeroCopyOutputStream& output, const void* data, std::size_t count) {
        const auto* ptr = static_cast<const uint8_t*>(data);
        int remaining = static_cast<int>(count);
        while (remaining > 0) {
            void* chunk = nullptr;
            int size = 0;
            if (!output.Next(&chunk, &size)) {
                throw std::runtime_error("Write failed");
            }
            const int toCopy = std::min(remaining, size);
            std::memcpy(chunk, ptr, toCopy);
            ptr += toCopy;
            remaining -= toCopy;
            if (toCopy < size) {
                output.BackUp(size - toCopy);
            }
        }
    }

    static void writeTextureAsset(google::protobuf::io::ZeroCopyOutputStream& output, uint32_t width, uint32_t height,
                                  std::span<const uint8_t> pixels) {
        Asset::TextureHeader proto;
        proto.set_width(width);
        proto.set_height(height);
        std::string headerBytes;
        proto.SerializeToString(&headerBytes);

        const uint32_t magic = 0x58544556;
        const uint32_t headerSize = static_cast<uint32_t>(headerBytes.size());

        writeHelper(output, &magic, sizeof(magic));
        writeHelper(output, &headerSize, sizeof(headerSize));
        writeHelper(output, headerBytes.data(), headerSize);
        writeHelper(output, pixels.data(), pixels.size_bytes());
    }

    static void writeMeshAsset(google::protobuf::io::ZeroCopyOutputStream& output,
                               std::span<const Asset::Mesh::Vertex> vertices,
                               std::span<const uint32_t> indices) {
        Asset::MeshHeader proto;
        proto.set_vertex_count(static_cast<uint32_t>(vertices.size()));
        proto.set_index_count(static_cast<uint32_t>(indices.size()));
        std::string headerBytes;
        proto.SerializeToString(&headerBytes);

        const uint32_t magic = 0x484D4556;
        const uint32_t headerSize = static_cast<uint32_t>(headerBytes.size());

        writeHelper(output, &magic, sizeof(magic));
        writeHelper(output, &headerSize, sizeof(headerSize));
        writeHelper(output, headerBytes.data(), headerSize);
        if (!vertices.empty()) {
            writeHelper(output, vertices.data(), vertices.size_bytes());
        }
        if (!indices.empty()) {
            writeHelper(output, indices.data(), indices.size_bytes());
        }
    }

protected:
    std::filesystem::path tempDir_;
    std::string tempPath_;
};

TEST_F(AssetTestFixture, ModelRoundTrip) {
    Asset::Model source;
    auto* part1 = source.add_parts();
    part1->set_mesh("meshes/cube.vemesh");
    part1->set_material("materials/cube.vematerial");
    for (float val : {1.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f}) {
        part1->add_transform(val);
    }

    auto* part2 = source.add_parts();
    part2->set_mesh("meshes/sphere.vemesh");
    part2->set_material("materials/sphere.vematerial");

    std::string serialized;
    ASSERT_TRUE(source.SerializeToString(&serialized));

    Asset::Model loaded;
    ASSERT_TRUE(loaded.ParseFromString(serialized));

    ASSERT_EQ(loaded.parts_size(), 2);
    EXPECT_EQ(loaded.parts(0).mesh(), "meshes/cube.vemesh");
    EXPECT_EQ(loaded.parts(0).material(), "materials/cube.vematerial");
    EXPECT_EQ(loaded.parts(0).transform_size(), 16);
    EXPECT_FLOAT_EQ(loaded.parts(0).transform(0), 1.0f);

    EXPECT_EQ(loaded.parts(1).mesh(), "meshes/sphere.vemesh");
    EXPECT_EQ(loaded.parts(1).material(), "materials/sphere.vematerial");
    EXPECT_EQ(loaded.parts(1).transform_size(), 0);
}

TEST_F(AssetTestFixture, MaterialRoundTrip) {
    Asset::Material source;
    source.mutable_albedo()->set_x(0.2f);
    source.mutable_albedo()->set_y(0.4f);
    source.mutable_albedo()->set_z(0.6f);
    source.mutable_albedo()->set_w(0.8f);
    source.set_specular(0.25f);
    source.set_metallic(0.5f);
    source.set_roughness(0.75f);
    source.mutable_emissive()->set_x(1.0f);
    source.mutable_emissive()->set_y(2.0f);
    source.mutable_emissive()->set_z(3.0f);
    source.set_albedo_texture("textures/albedo.vetexture");
    source.set_normal_texture("textures/normal.vetexture");
    source.set_metallic_roughness_texture("textures/mr.vetexture");
    source.set_lit(false);
    source.set_alpha_mode(Asset::ALPHA_MODE_BLEND);

    std::string serialized;
    ASSERT_TRUE(source.SerializeToString(&serialized));

    Asset::Material loaded;
    ASSERT_TRUE(loaded.ParseFromString(serialized));

    EXPECT_FLOAT_EQ(loaded.albedo().x(), 0.2f);
    EXPECT_FLOAT_EQ(loaded.albedo().y(), 0.4f);
    EXPECT_FLOAT_EQ(loaded.albedo().z(), 0.6f);
    EXPECT_FLOAT_EQ(loaded.albedo().w(), 0.8f);
    EXPECT_FLOAT_EQ(loaded.specular(), 0.25f);
    EXPECT_FLOAT_EQ(loaded.metallic(), 0.5f);
    EXPECT_FLOAT_EQ(loaded.roughness(), 0.75f);
    EXPECT_FLOAT_EQ(loaded.emissive().x(), 1.0f);
    EXPECT_FLOAT_EQ(loaded.emissive().y(), 2.0f);
    EXPECT_FLOAT_EQ(loaded.emissive().z(), 3.0f);
    EXPECT_EQ(loaded.albedo_texture(), "textures/albedo.vetexture");
    EXPECT_EQ(loaded.normal_texture(), "textures/normal.vetexture");
    EXPECT_EQ(loaded.metallic_roughness_texture(), "textures/mr.vetexture");
    EXPECT_FALSE(loaded.lit());
    EXPECT_EQ(loaded.alpha_mode(), Asset::ALPHA_MODE_BLEND);
}

TEST_F(AssetTestFixture, TextureIsZeroCopyInputStreamOfPixels) {
    const std::vector<uint8_t> pixels = {128, 64, 0, 128, 0, 0, 0, 0};

    std::string buffer;
    {
        google::protobuf::io::StringOutputStream output(&buffer);
        writeTextureAsset(output, 2, 1, pixels);
    }

    google::protobuf::io::ArrayInputStream input(buffer.data(), static_cast<int>(buffer.size()));
    Asset::Texture texture(input);

    EXPECT_EQ(texture.width(), 2);
    EXPECT_EQ(texture.height(), 1);
    EXPECT_EQ(texture.header().width(), 2);
    EXPECT_EQ(texture.header().height(), 1);
    EXPECT_EQ(texture.payloadSize(), 8);

    std::vector<uint8_t> streamed;
    const void* chunk = nullptr;
    int chunkSize = 0;
    while (texture.Next(&chunk, &chunkSize)) {
        if (chunkSize > 0) {
            streamed.insert(streamed.end(), static_cast<const uint8_t*>(chunk),
                            static_cast<const uint8_t*>(chunk) + chunkSize);
        }
    }

    EXPECT_EQ(streamed, pixels);
    EXPECT_EQ(texture.ByteCount(), 8);
    EXPECT_FALSE(texture.Next(&chunk, &chunkSize));
}

TEST_F(AssetTestFixture, MeshIsZeroCopyInputStreamOfVerticesAndIndices) {
    const std::vector<Asset::Mesh::Vertex> vertices = {
        {{{1.0f, -2.0f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.25f, 0.75f}}}
    };
    const std::vector<std::uint32_t> indices = {0, 0, 0};

    std::string buffer;
    {
        google::protobuf::io::StringOutputStream output(&buffer);
        writeMeshAsset(output, vertices, indices);
    }

    google::protobuf::io::ArrayInputStream input(buffer.data(), static_cast<int>(buffer.size()));
    Asset::Mesh mesh(input);

    EXPECT_EQ(mesh.vertexCount(), 1);
    EXPECT_EQ(mesh.indexCount(), 3);
    EXPECT_EQ(mesh.header().vertex_count(), 1);
    EXPECT_EQ(mesh.header().index_count(), 3);
    const auto expectedSize = sizeof(Asset::Mesh::Vertex) + 3 * sizeof(uint32_t);
    EXPECT_EQ(mesh.payloadSize(), expectedSize);

    std::vector<uint8_t> streamed;
    const void* chunk = nullptr;
    int chunkSize = 0;
    while (mesh.Next(&chunk, &chunkSize)) {
        if (chunkSize > 0) {
            streamed.insert(streamed.end(), static_cast<const uint8_t*>(chunk),
                            static_cast<const uint8_t*>(chunk) + chunkSize);
        }
    }

    EXPECT_EQ(streamed.size(), expectedSize);
    EXPECT_EQ(mesh.ByteCount(), static_cast<int64_t>(expectedSize));
    EXPECT_FALSE(mesh.Next(&chunk, &chunkSize));
}

TEST_F(AssetTestFixture, PhysFileStreamIntegration) {
    const std::vector<uint8_t> pixels = {255, 128, 64, 255};
    {
        Volcano::PhysOutputStream fileOut("image.vetexture");
        writeTextureAsset(fileOut, 1, 1, pixels);
    }

    {
        Volcano::PhysInputStream fileIn("image.vetexture");
        Asset::Texture texture(fileIn);
        EXPECT_EQ(texture.width(), 1);
        EXPECT_EQ(texture.height(), 1);

        const void* chunk = nullptr;
        int chunkSize = 0;
        ASSERT_TRUE(texture.Next(&chunk, &chunkSize));
        EXPECT_EQ(chunkSize, 4);
        EXPECT_EQ(std::memcmp(chunk, pixels.data(), 4), 0);
    }
}

TEST_F(AssetTestFixture, ValidationRejectsInvalidStream) {
    const uint32_t badMagic = 0x12345678;
    std::string buffer(reinterpret_cast<const char*>(&badMagic), sizeof(badMagic));
    google::protobuf::io::ArrayInputStream input(buffer.data(), static_cast<int>(buffer.size()));
    EXPECT_THROW(Asset::Texture texture(input), std::invalid_argument);
}

} // namespace
