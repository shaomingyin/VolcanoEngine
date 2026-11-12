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
#include <Volcano/Asset/Common.h>
#include <Volcano/Framework/Resources.h>

namespace {
namespace Resources = Volcano::Framework::Resources;
using Volcano::Asset::Material;
using Volcano::Asset::Model;

class ResourceCache : public ::testing::Test {
protected:
    void SetUp() override {
        root_ = std::filesystem::temp_directory_path() /
                ("ve_resource_paths_" +
                 std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
        std::filesystem::create_directories(root_ / "materials");
        std::filesystem::create_directories(root_ / "models");
        ASSERT_NE(PHYSFS_init("ResourceCache"), 0);
        ASSERT_NE(PHYSFS_mount(root_.string().c_str(), nullptr, 1), 0);
        ASSERT_NE(PHYSFS_setWriteDir(root_.string().c_str()), 0);
        writeMaterial();
    }

    void TearDown() override {
        Resources::clear();
        EXPECT_NE(PHYSFS_deinit(), 0);
        std::error_code error;
        std::filesystem::remove_all(root_, error);
    }

    static void writeMaterial(const std::string& path = "materials/sample.vematerial", float roughness = 1.0f) {
        Volcano::PhysOutputStream file(path);
        Material mat;
        mat.set_roughness(roughness);
        mat.set_albedo_texture("textures/albedo.vetexture");
        ASSERT_TRUE(mat.SerializeToZeroCopyStream(&file));
    }

    static void writeModel(const std::string& path = "sample.vemodel") {
        Volcano::PhysOutputStream file(path);
        Model model;
        auto* part = model.add_parts();
        part->set_mesh("mesh.vemesh");
        part->set_material("materials/sample.vematerial");
        ASSERT_TRUE(model.SerializeToZeroCopyStream(&file));
    }

protected:
    std::filesystem::path root_;
};

TEST_F(ResourceCache, ResolvesAllPathsFromThePhysfsRootAndReusesLiveObjects) {
    auto resource = Resources::loadMaterial("/materials/sample.vematerial");
    EXPECT_EQ(Resources::loadMaterial("materials/sample.vematerial"), resource);
    EXPECT_EQ(Resources::loadMaterial("/materials/./sample.vematerial"), resource);
    EXPECT_EQ(Resources::loadMaterial("./materials/sample.vematerial"), resource);
    EXPECT_FLOAT_EQ(resource->roughness(), 1.0f);
    EXPECT_EQ(resource->albedo_texture(), "materials/textures/albedo.vetexture");

    std::filesystem::remove(root_ / "materials/sample.vematerial");
    EXPECT_EQ(Resources::loadMaterial("/materials/sample.vematerial"), resource);
    EXPECT_THROW(Resources::loadMaterial("/materials/missing.vematerial"), std::runtime_error);
}

TEST_F(ResourceCache, NeverTreatsVirtualPathsAsHostFilesystemPaths) {
    const auto hostPath = (root_ / "materials/sample.vematerial").generic_string();
    ASSERT_TRUE(std::filesystem::is_regular_file(hostPath));
    EXPECT_THROW(Resources::loadMaterial(hostPath), std::runtime_error);
}

TEST_F(ResourceCache, ReloadsTheFileAfterLastOwnerReleases) {
    auto first = Resources::loadMaterial("/materials/sample.vematerial");
    std::weak_ptr<Material> weak = first;
    first.reset();
    EXPECT_TRUE(weak.expired());

    writeMaterial("materials/sample.vematerial", 0.5f);
    auto reloaded = Resources::loadMaterial("/materials/sample.vematerial");
    EXPECT_FLOAT_EQ(reloaded->roughness(), 0.5f);
}

TEST_F(ResourceCache, ClearsLookupsWithoutInvalidatingHandles) {
    auto material = Resources::loadMaterial("/materials/sample.vematerial");
    writeModel("sample.vemodel");
    auto model = Resources::loadModel("/sample.vemodel");

    Resources::clear();

    EXPECT_NE(Resources::loadMaterial("/materials/sample.vematerial"), material);
    EXPECT_NE(Resources::loadModel("/sample.vemodel"), model);
    EXPECT_FLOAT_EQ(material->roughness(), 1.0f);
    ASSERT_EQ(model->parts_size(), 1);
    EXPECT_EQ(model->parts(0).mesh(), "mesh.vemesh");
    EXPECT_EQ(model->parts(0).material(), "materials/sample.vematerial");
}

TEST_F(ResourceCache, RejectsInvalidPaths) {
    for (const auto* path :
         {"", "/", ".", "../sample", "/materials/../sample", "C:/sample", "a\\b", "generated:screen"}) {
        EXPECT_THROW(Resources::loadMaterial(path), std::invalid_argument);
    }
    std::string nul = "/materials/sample";
    nul.push_back('\0');
    nul += ".vematerial";
    EXPECT_THROW(Resources::loadMaterial(nul), std::invalid_argument);
}

TEST_F(ResourceCache, FileFailuresAreNotCachedAndCanBeRetried) {
    std::filesystem::remove(root_ / "materials/sample.vematerial");
    EXPECT_THROW(Resources::loadMaterial("/materials/sample.vematerial"), std::runtime_error);
    writeMaterial();
    EXPECT_NO_THROW(Resources::loadMaterial("/materials/sample.vematerial"));
}

TEST_F(ResourceCache, ConcurrentLoadsForSamePathCoordinateCleanly) {
    constexpr int threadsCount = 4;
    std::vector<std::thread> workers;
    std::vector<std::shared_ptr<Material>> results(threadsCount);
    std::barrier sync(threadsCount);

    for (int i = 0; i < threadsCount; ++i) {
        workers.emplace_back([&, i] {
            sync.arrive_and_wait();
            results[i] = Resources::loadMaterial("/materials/sample.vematerial");
        });
    }

    for (auto& worker : workers) {
        worker.join();
    }

    for (int i = 1; i < threadsCount; ++i) {
        EXPECT_EQ(results[0], results[i]);
    }
}

} // namespace
