#include <array>
#include <barrier>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include <gtest/gtest.h>
#include <physfs.h>

#include <Volcano/PhysFile.h>
#include <Volcano/Schema/Material.h>
#include <Volcano/Schema/Texture.h>
#include <Volcano/Framework/Resources.h>

namespace {
namespace Resources = Volcano::Framework::Resources;
using Volcano::PhysFile;
using Volcano::Schema::Material;
using Volcano::Schema::Texture;

class ResourceCache : public ::testing::Test {
protected:
    void SetUp() override {
        root_ = std::filesystem::temp_directory_path() /
                ("ve_resource_paths_" +
                 std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
        std::filesystem::create_directories(root_ / "textures");
        ASSERT_NE(PHYSFS_init("ResourceCache"), 0);
        ASSERT_NE(PHYSFS_mount(root_.string().c_str(), nullptr, 1), 0);
        ASSERT_NE(PHYSFS_setWriteDir(root_.string().c_str()), 0);
        writeTexture();
    }

    void TearDown() override {
        Resources::clear();
        EXPECT_NE(PHYSFS_deinit(), 0);
        std::error_code error;
        std::filesystem::remove_all(root_, error);
    }

    static void writeTexture(std::uint8_t red = 1) {
        PhysFile file("textures/image.vetexture", PhysFile::OpenMode::WriteOnly);
        Texture{1, 1, {red, 0, 0, 255}}.write(file);
    }

protected:
    std::filesystem::path root_;
};

TEST_F(ResourceCache, ResolvesAllPathsFromThePhysfsRootAndReusesLiveObjects) {
    auto resource = Resources::loadTextureData("/textures/image.vetexture");
    EXPECT_EQ(Resources::loadTextureData("textures/image.vetexture"), resource);
    EXPECT_EQ(Resources::loadTextureData("/textures/./image.vetexture"), resource);
    EXPECT_EQ(Resources::loadTextureData("./textures/image.vetexture"), resource);
    EXPECT_THROW(Resources::loadMaterial("/textures/image.vetexture"), std::runtime_error);
    std::filesystem::remove(root_ / "textures/image.vetexture");
    EXPECT_EQ(Resources::loadTextureData("/textures/image.vetexture"), resource);
    EXPECT_THROW(Resources::loadTextureData("/textures/missing.vetexture"), std::runtime_error);
}

TEST_F(ResourceCache, NeverTreatsVirtualPathsAsHostFilesystemPaths) {
    const auto hostPath = (root_ / "textures/image.vetexture").generic_string();
    ASSERT_TRUE(std::filesystem::is_regular_file(hostPath));
    EXPECT_THROW(Resources::loadTextureData(hostPath), std::runtime_error);
}

TEST_F(ResourceCache, ReloadsTheFileAfterLastOwnerReleases) {
    auto first = Resources::loadTextureData("/textures/image.vetexture");
    std::weak_ptr<Texture> weak = first;
    first.reset();
    EXPECT_TRUE(weak.expired());
    writeTexture(2);
    auto reloaded = Resources::loadTextureData("/textures/image.vetexture");
    EXPECT_EQ(reloaded->pixels[0], 2);
}

TEST_F(ResourceCache, ClearsLookupsWithoutInvalidatingHandles) {
    auto texture = Resources::loadTextureData("/textures/image.vetexture");
    {
        PhysFile file("material.vematerial", PhysFile::OpenMode::WriteOnly);
        Material{}.write(file);
    }
    auto material = Resources::loadMaterial("/material.vematerial");
    Resources::clear();
    EXPECT_NE(Resources::loadTextureData("/textures/image.vetexture"), texture);
    EXPECT_NE(Resources::loadMaterial("/material.vematerial"), material);
    EXPECT_EQ(texture->pixels[0], 1);
    EXPECT_EQ(material->roughness, 1.0f);
}

TEST_F(ResourceCache, RejectsInvalidPaths) {
    for (const auto* path :
         {"", "/", ".", "../image", "/textures/../image", "C:/image", "a\\b", "generated:screen"}) {
        EXPECT_THROW(Resources::loadTextureData(path), std::invalid_argument);
    }
    std::string nul = "/textures/image";
    nul.push_back('\0');
    nul += ".vetexture";
    EXPECT_THROW(Resources::loadTextureData(nul), std::invalid_argument);
}

TEST_F(ResourceCache, FileFailuresAreNotCachedAndCanBeRetried) {
    std::filesystem::remove(root_ / "textures/image.vetexture");
    EXPECT_THROW(Resources::loadTextureData("/textures/image.vetexture"), std::runtime_error);
    {
        PhysFile file("textures/image.vetexture", PhysFile::OpenMode::WriteOnly);
        file.writeAll(Volcano::ByteArray{0, 1, 2});
    }
    EXPECT_THROW(Resources::loadTextureData("/textures/image.vetexture"), std::runtime_error);
    writeTexture();
    EXPECT_NE(Resources::loadTextureData("/textures/image.vetexture"), nullptr);
}

TEST_F(ResourceCache, ConcurrentLoadsPublishTheSameLiveObject) {
    constexpr std::size_t count = 4;
    std::barrier rendezvous(static_cast<std::ptrdiff_t>(count));
    std::array<std::shared_ptr<Texture>, count> results;
    {
        std::vector<std::jthread> threads;
        for (std::size_t i = 0; i < count; ++i) {
            threads.emplace_back([&, i] {
                rendezvous.arrive_and_wait();
                results[i] = Resources::loadTextureData("/textures/image.vetexture");
            });
        }
    }
    for (const auto& result : results) {
        EXPECT_EQ(result, results[0]);
    }
    EXPECT_EQ(Resources::loadTextureData("/textures/image.vetexture"), results[0]);
}
} // namespace
