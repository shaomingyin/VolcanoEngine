#define GLFW_INCLUDE_NONE
#include <array>
#include <chrono>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include <GL/gl.hpp>
#include <GLFW/glfw3.h>
#include <gtest/gtest.h>

#include <Volcano/PhysFile.h>
#include <Volcano/Graphics/FrameBuffer.h>
#include <Volcano/Schema/Material.h>
#include <Volcano/Schema/Mesh.h>
#include <Volcano/Schema/Model.h>
#include <Volcano/Schema/Texture.h>
#include <Volcano/World/Inherent.h>
#include <Volcano/World/Model.h>
#include <Volcano/World/OrthographicCamera.h>
#include <Volcano/World/PointLight.h>
#include <Volcano/Framework/Resources.h>
#include <Volcano/Framework/Client.h>

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
        emplace<Transform>(entity).setTranslation({1, 2, 3});
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


public:
    bool failLoad = false;
    std::vector<int> order;

protected:
    async::task<void> onLoadScene(const World::Scene& scene) override {
        return Local::onLoadScene(scene).then(scheduler(), [this] {
            if (failLoad) {
                throw std::runtime_error("GPU load failure");
            }
        });
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
        ::testing::FLAGS_gtest_death_test_style = "threadsafe";
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
        Framework::Resources::clear();
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
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    EXPECT_DEATH(client.render(target, entt::null), "[Aa]ssert");
#endif
    client.unload();
    EXPECT_FALSE(client.view().ready());
    EXPECT_TRUE(client.view().items().empty());
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    EXPECT_DEATH(client.render(target, entt::null), "[Aa]ssert");
#endif
}

TEST_F(FrameworkLocal, GlobalResourcesKeepGpuCachesSeparateBetweenWindowContexts) {
    Client first;
    first.load([] { return std::make_unique<Scene>(); });
    const auto* shared_model = Framework::Resources::loadModel("assets/model.vemodel").get();
    first.render(first.scene().player());
    const auto* first_mesh = &first.view().items()[0].mesh.get();
    {
        Client second;
        second.load([] { return std::make_unique<Scene>(); });
        EXPECT_EQ(Framework::Resources::loadModel("assets/model.vemodel").get(), shared_model);
        second.render(second.scene().player());
        EXPECT_NE(&second.view().items()[0].mesh.get(), first_mesh);
        second.unload();
        first.window().makeCurrent();
        first.render(first.scene().player());
        EXPECT_EQ(&first.view().items()[0].mesh.get(), first_mesh);
        EXPECT_EQ(glGetError(), GL_NO_ERROR);
    }
    first.window().makeCurrent();
    first.render(first.scene().player());
    EXPECT_EQ(&first.view().items()[0].mesh.get(), first_mesh);
    EXPECT_EQ(glGetError(), GL_NO_ERROR);
}

TEST_F(FrameworkLocal, RollsBackGpuLoadAndRequiresExplicitSynchronization) {
    Client client;
    client.failLoad = true;
    EXPECT_THROW(client.load([] { return std::make_unique<Scene>(); }), std::runtime_error);
    EXPECT_EQ(client.state(), Framework::Base::State::Idle);

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
    client.scene().get<World::Model>(static_cast<Scene&>(client.scene()).model).setSource("assets/other.vemodel");
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    EXPECT_DEATH(client.render(target, client.scene().player()), "[Aa]ssert");
#endif
    Schema::Model model;
    model.parts.push_back({"mesh.vemesh", "material.vematerial", Eigen::Affine3f::Identity()});
    write("assets/other.vemodel", model);
    EXPECT_NO_THROW(client.synchronizeResources());
    EXPECT_NO_THROW(client.render(target, client.scene().player()));
    EXPECT_EQ(glGetError(), GL_NO_ERROR);
}
}

namespace {
TEST_F(FrameworkLocal, ConcurrentAsyncLoadsUseTheirOwningWindowQueues) {
    Framework::Local first(32, 32, "First async window");
    Framework::Local second(32, 32, "Second async window");
    auto firstTask = first.loadAsync([] { return std::make_unique<Scene>(); });
    auto secondTask = second.loadAsync([] { return std::make_unique<Scene>(); });
    const auto deadline = Clock::now() + std::chrono::seconds(5);
    while ((!firstTask.ready() || !secondTask.ready()) && Clock::now() < deadline) {
        first.update({});
        EXPECT_EQ(glfwGetCurrentContext(), first.window().handle());
        second.update({});
        EXPECT_EQ(glfwGetCurrentContext(), second.window().handle());
        std::this_thread::yield();
    }
    ASSERT_TRUE(firstTask.ready());
    ASSERT_TRUE(secondTask.ready());
    EXPECT_NO_THROW(firstTask.get());
    EXPECT_NO_THROW(secondTask.get());
    first.window().makeCurrent();
    first.render(first.scene().player());
    const auto* mesh = &first.view().items()[0].mesh.get();
    EXPECT_EQ(glGetError(), GL_NO_ERROR);
    second.window().makeCurrent();
    second.render(second.scene().player());
    EXPECT_NE(&second.view().items()[0].mesh.get(), mesh);
    EXPECT_EQ(glGetError(), GL_NO_ERROR);
}

class DeferredLocal final : public Framework::Local {
public:
    DeferredLocal() : Framework::Local(32, 32, "Cancelled GL loading") {}

    ~DeferredLocal() override {
        cancelLoad();
        if (!gateTask.ready()) {
            gate.set();
        }
        try {
            waitForLoad();
        } catch (const async::task_canceled&) {
        }
    }

public:
    async::event_task<void> gate;
    async::shared_task<void> gateTask = gate.get_task().share();
    bool waiting = false;
    bool defer = true;

protected:
    async::task<void> onLoadScene(const World::Scene& scene) override {
        return Local::onLoadScene(scene).then(scheduler(), [this] {
            waiting = true;
            return defer ? gateTask.then(async::inline_scheduler(), [] {}) : async::make_task();
        });
    }
};

TEST_F(FrameworkLocal, CancelsPreparedGpuResourcesBeforePublishingAndAllowsRetry) {
    DeferredLocal local;
    auto task = local.loadAsync([] { return std::make_unique<Scene>(); });
    const auto deadline = Clock::now() + std::chrono::seconds(5);
    while (!local.waiting && Clock::now() < deadline) {
        local.update({});
        std::this_thread::yield();
    }
    ASSERT_TRUE(local.waiting);
    local.cancelLoad();
    EXPECT_FALSE(task.ready());
    local.gate.set();
    EXPECT_THROW(local.waitForLoad(), async::task_canceled);
    EXPECT_THROW(task.get(), async::task_canceled);
    EXPECT_EQ(local.state(), Framework::Base::State::Idle);
    std::filesystem::remove(root_ / "assets/texture.vetexture");
    // A committed GPU cache would hide this missing file on retry.
    local.defer = false;
    EXPECT_THROW(local.load([] { return std::make_unique<Scene>(); }), std::runtime_error);
    write("assets/texture.vetexture", Schema::Texture{1, 1, {128, 0, 0, 128}});
    EXPECT_NO_THROW(local.load([] { return std::make_unique<Scene>(); }));
    EXPECT_FALSE(local.loadCancelled());
}

TEST_F(FrameworkLocal, CancelsOutstandingTasksBeforeDestroyingGlContext) {
    async::task<void> task;
    {
        auto local = std::make_unique<DeferredLocal>();
        task = local->loadAsync([] { return std::make_unique<Scene>(); });
        const auto deadline = Clock::now() + std::chrono::seconds(5);
        while (!local->waiting && Clock::now() < deadline) {
            local->update({});
            std::this_thread::yield();
        }
        ASSERT_TRUE(local->waiting);
    }
    ASSERT_TRUE(task.ready());
    EXPECT_THROW(task.get(), async::task_canceled);
    EXPECT_EQ(gl3wProcs, nullptr);
}
}

namespace {
TEST_F(FrameworkLocal, LoadsGraphicsResourcesDirectlyWithoutSceneOrFactory) {
    Framework::Local first(32, 32, "Direct resource loading");
    auto mesh = Framework::Resources::loadMesh("/assets/mesh.vemesh");
    auto material = Framework::Resources::loadMaterial("assets/material.vematerial");
    auto texture = Framework::Resources::loadTexture("/assets/texture.vetexture");
    EXPECT_EQ(first.state(), Framework::Base::State::Idle);
    EXPECT_FALSE(mesh->empty());
    EXPECT_EQ(material->albedoTexture, "assets/texture.vetexture");
    EXPECT_EQ(texture->size(), (Size{1, 1}));
    EXPECT_EQ(Framework::Resources::loadMesh("/assets/./mesh.vemesh"), mesh);
    EXPECT_EQ(Framework::Resources::loadTexture("./assets/texture.vetexture"), texture);
    {
        Framework::Local second(32, 32, "Separate resource context");
        auto otherMesh = Framework::Resources::loadMesh("assets/mesh.vemesh");
        EXPECT_NE(otherMesh, mesh);
        EXPECT_EQ(Framework::Resources::loadMaterial("assets/material.vematerial"), material);
        EXPECT_EQ(glGetError(), GL_NO_ERROR);
    }
    first.window().makeCurrent();
    EXPECT_EQ(Framework::Resources::loadTexture("assets/texture.vetexture"), texture);
    EXPECT_EQ(texture->size(), (Size{1, 1}));
    std::weak_ptr<Graphics::Texture> weak = texture;
    texture.reset();
    EXPECT_TRUE(weak.expired());
    EXPECT_EQ(glGetError(), GL_NO_ERROR);
}
}

namespace {
TEST_F(FrameworkLocal, SynchronizationReleasesUnusedGpuHandlesAfterClearingView) {
    Client client;
    client.load([] { return std::make_unique<Scene>(); });
    client.render(client.scene().player());
    std::weak_ptr<Graphics::Mesh> mesh = Framework::Resources::loadMesh("assets/mesh.vemesh");
    std::weak_ptr<Graphics::Texture> texture = Framework::Resources::loadTexture("assets/texture.vetexture");
    ASSERT_FALSE(mesh.expired());
    ASSERT_FALSE(texture.expired());

    client.scene().clear<World::Model>();
    client.synchronizeResources();

    EXPECT_TRUE(client.view().items().empty());
    EXPECT_TRUE(mesh.expired());
    EXPECT_TRUE(texture.expired());
    EXPECT_EQ(glGetError(), GL_NO_ERROR);
}
} // namespace
