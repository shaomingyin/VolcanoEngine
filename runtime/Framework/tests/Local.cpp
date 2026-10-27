#define GLFW_INCLUDE_NONE
#include <array>
#include <chrono>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <GLFW/glfw3.h>
#include <gtest/gtest.h>

#include <Volcano/Framework/Client.h>
#include <Volcano/Graphics/FrameBuffer.h>
#include <Volcano/PhysFile.h>
#include <Volcano/Schema/Material.h>
#include <Volcano/Schema/Mesh.h>
#include <Volcano/Schema/Model.h>
#include <Volcano/Schema/Texture.h>
#include <Volcano/World/Inherent.h>
#include <Volcano/World/Model.h>
#include <Volcano/World/OrthographicCamera.h>
#include <Volcano/World/PointLight.h>

namespace {
using namespace Volcano;

class Scene final : public World::Scene {
public:
    Scene() {
        camera = create();
        emplace<World::OrthographicCamera>(camera, -1, 1, -1, 1, 0.1f, 10);
        model = create();
        emplace<World::Model>(model, "assets/model.vemodel");
        emplace<World::Inherent>(model);
        auto entity = create();
        emplace<World::PointLight>(entity);
        emplace<Transform>(entity).translation() = {1, 2, 3};
    }

    entt::entity player() const noexcept override {
        return camera;
    }

public:
    entt::entity camera;
    entt::entity model;
    std::vector<int>* order = nullptr;

protected:
    void onUpdate(Clock::duration) noexcept override {
        if (order != nullptr) {
            order->push_back(3);
        }
    }
};

class Client final : public Framework::Client {
public:
    Client() : Framework::Client(32, 32, "FrameworkLocalCheck") {
    }

    using Framework::Base::modelResource;

public:
    bool failLoad = false;
    std::vector<int> order;

protected:
    void onLoadResources(const World::Scene& scene) override {
        Local::onLoadResources(scene);
        if (failLoad) {
            throw std::runtime_error("GPU load failure");
        }
    }

    void onInputUpdate(Clock::duration) override {
        order.push_back(1);
    }

    void onNetworkUpdate(Clock::duration) override {
        order.push_back(2);
    }

    void onAudioUpdate(Clock::duration) override {
        order.push_back(4);
    }
};

TEST(FrameworkLocalWindow, OwnsWindowContextAndSupportsRecreation) {
    EXPECT_THROW((Framework::Local(0, 32, "invalid")), std::invalid_argument);
    EXPECT_THROW((Framework::Local(32, 32, nullptr)), std::invalid_argument);
    for (int iteration = 0; iteration < 2; ++iteration) {
        {
            Framework::Local local(32, 32, "Local window lifecycle");
            EXPECT_EQ(glfwGetCurrentContext(), local.window().handle());
            EXPECT_GT(local.window().width(), 0);
            EXPECT_GT(local.window().height(), 0);
            EXPECT_GE(glfwGetWindowAttrib(local.window().handle(), GLFW_CONTEXT_VERSION_MAJOR), 3);
            glfwMakeContextCurrent(nullptr);
            local.window().makeCurrent();
            EXPECT_EQ(glfwGetCurrentContext(), local.window().handle());
            EXPECT_EQ(glGetError(), GL_NO_ERROR);
        }
        EXPECT_EQ(gl3wProcs, nullptr);
    }
}

class FrameworkLocal : public ::testing::Test {
protected:
    void SetUp() override {
        root_ = std::filesystem::temp_directory_path() /
            ("volcano-framework-local-" + std::to_string(Clock::now().time_since_epoch().count()));
        std::filesystem::create_directories(root_ / "assets");
        ASSERT_NE(PHYSFS_init("FrameworkLocalCheck"), 0);
        ASSERT_NE(PHYSFS_mount(root_.string().c_str(), nullptr, 1), 0);
        ASSERT_NE(PHYSFS_setWriteDir(root_.string().c_str()), 0);
        Schema::Mesh mesh;
        mesh.vertices = {{{-0.8f, -0.8f, -2}}, {{0.8f, -0.8f, -2}}, {{0, 0.8f, -2}}};
        mesh.indices = {0, 1, 2};
        write("assets/mesh.vemesh", mesh);
        Schema::Material material;
        material.lit = false;
        material.albedoTexture = "texture.vetexture";
        write("assets/material.vematerial", material);
        write("assets/texture.vetexture", Schema::Texture{1, 1, {128, 0, 0, 128}});
        Schema::Model model;
        Eigen::Affine3f shear = Eigen::Affine3f::Identity();
        shear.linear()(0, 1) = 0.25f;
        model.parts.push_back({"mesh.vemesh", "material.vematerial", shear});
        write("assets/model.vemodel", model);
    }

    void TearDown() override {
        EXPECT_NE(PHYSFS_deinit(), 0);
        std::error_code error;
        std::filesystem::remove_all(root_, error);
    }

    template<typename Resource>
    void write(const char* path, const Resource& resource) {
        PhysFile file(path, PhysFile::OpenMode::WriteOnly);
        resource.write(file);
    }

protected:
    std::filesystem::path root_;
};

TEST_F(FrameworkLocal, LoadsRendersExtractsSceneAndReusesGpuResources) {
    Client client;
    client.load([&] {
        auto scene = std::make_unique<Scene>();
        scene->order = &client.order;
        return scene;
    });
    Graphics::FrameBuffer target(16, 16);
    client.render(target, client.scene().player());
    ASSERT_EQ(client.view().items().size(), 1);
    const auto* mesh = &client.view().items()[0].mesh.get();
    const auto* texture = client.view().items()[0].material.texture;
    EXPECT_FLOAT_EQ(client.view().opaqueItems()[0].model.linear()(0, 1), 0.25f);
    ASSERT_EQ(client.view().lights().size(), 1);
    EXPECT_EQ(client.view().lights()[0].position, Eigen::Vector3f(1, 2, 3));
    target.bind();
    std::array<std::uint8_t, 4> pixel{};
    glReadPixels(8, 8, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel.data());
    EXPECT_NEAR(pixel[0], 128, 2);
    EXPECT_EQ(pixel[1], 0);
    EXPECT_EQ(glGetError(), GL_NO_ERROR);

    client.play();
    client.update(std::chrono::milliseconds(1));
    EXPECT_EQ(client.order, (std::vector<int>{1, 2, 3, 4}));
    client.pause();
    client.order.clear();
    client.update({});
    EXPECT_EQ(client.order, (std::vector<int>{1, 2, 4}));
    std::filesystem::remove_all(root_ / "assets");
    EXPECT_NO_THROW(client.synchronizeResources());
    EXPECT_NO_THROW(client.stop());
    client.render(target, client.scene().player());
    EXPECT_EQ(&client.view().items()[0].mesh.get(), mesh);
    EXPECT_EQ(client.view().items()[0].material.texture, texture);
    auto& scene = static_cast<Scene&>(client.scene());
    scene.get<World::Inherent>(scene.model).hide();
    client.render(target, scene.camera);
    EXPECT_TRUE(client.view().items().empty());
    EXPECT_THROW(client.render(target, entt::null), std::invalid_argument);
    client.unload();
    EXPECT_FALSE(client.view().ready());
    EXPECT_TRUE(client.view().items().empty());
    EXPECT_THROW(client.render(target, entt::null), std::logic_error);
}

TEST_F(FrameworkLocal, RollsBackGpuLoadAndRequiresExplicitSynchronization) {
    Client client;
    client.failLoad = true;
    EXPECT_THROW(client.load([] { return std::make_unique<Scene>(); }), std::runtime_error);
    EXPECT_EQ(client.state(), Framework::Base::State::Idle);
    EXPECT_THROW(client.modelResource("assets/model.vemodel"), std::logic_error);
    client.failLoad = false;
    std::filesystem::remove(root_ / "assets/texture.vetexture");
    EXPECT_THROW(client.load([] { return std::make_unique<Scene>(); }), std::runtime_error);
    write("assets/texture.vetexture", Schema::Texture{1, 1, {128, 0, 0, 128}});
    Schema::Material replacement;
    replacement.lit = false;
    replacement.albedo = {0.5f, 1, 1, 1};
    replacement.albedoTexture = "texture.vetexture";
    write("assets/material.vematerial", replacement);
    EXPECT_NO_THROW(client.load([] { return std::make_unique<Scene>(); }));
    Graphics::FrameBuffer target(16, 16);
    client.render(target, client.scene().player());
    ASSERT_EQ(client.view().items().size(), 1);
    EXPECT_EQ(client.view().items()[0].material.color, Colorf(0.5f, 1, 1, 1));
    client.scene().get<World::Model>(static_cast<Scene&>(client.scene()).model).source = "assets/other.vemodel";
    EXPECT_THROW(client.render(target, client.scene().player()), std::logic_error);
    Schema::Model model;
    model.parts.push_back({"mesh.vemesh", "material.vematerial", Eigen::Affine3f::Identity()});
    write("assets/other.vemodel", model);
    EXPECT_NO_THROW(client.synchronizeResources());
    EXPECT_NO_THROW(client.render(target, client.scene().player()));
    EXPECT_EQ(glGetError(), GL_NO_ERROR);
}
}
