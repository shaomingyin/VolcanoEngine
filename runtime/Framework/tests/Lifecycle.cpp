#include <atomic>
#include <chrono>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include <GL/gl3w.h>
#include <async++.h>
#include <gtest/gtest.h>

#include <Volcano/PhysFile.h>
#include <Volcano/Asset/Common.h>
#include <Volcano/Asset/Mesh.h>
#include <Volcano/Asset/Texture.h>
#include <Volcano/World/Inherent.h>
#include <Volcano/World/Model.h>
#include <Volcano/World/PerspectiveCamera.h>
#include <Volcano/World/PointLight.h>
#include <Volcano/World/RigidBody.h>
#include <Volcano/Framework/Drawing.h>
#include <Volcano/Framework/Resources.h>
#include <Volcano/Framework/Server.h>

namespace {
using namespace Volcano;
using Framework::Base;
using State = Base::State;

class Scene final : public World::Scene {
public:
    using World::Scene::Scene;

    entt::entity player() const noexcept override {
        return entt::null;
    }

public:
    int updates = 0;
    std::vector<int>* order = nullptr;

protected:
    void onUpdate(Clock::duration) noexcept override {
        ++updates;
        if (order != nullptr) {
            order->push_back(2);
        }
    }
};

struct GameProgress final {
    int ticks = 0;
};

class GameScene final : public World::Scene {
public:
    explicit GameScene(int& destructions)
        : destructions_(destructions) {
        player_ = create();
        emplace<GameProgress>(player_);
    }

    ~GameScene() override {
        ++destructions_;
    }

    entt::entity player() const noexcept override {
        return player_;
    }

protected:
    void onUpdate(Clock::duration) noexcept override {
        ++get<GameProgress>(player_).ticks;
    }

private:
    int& destructions_;
    entt::entity player_;
};

class FrameworkLifecycle : public ::testing::Test {
protected:
    void SetUp() override {
        ASSERT_NE(PHYSFS_init(nullptr), 0);
        root_ = std::filesystem::temp_directory_path() /
            ("volcano-lifecycle-" + std::to_string(Clock::now().time_since_epoch().count()));
        std::filesystem::create_directories(root_);
        ASSERT_NE(PHYSFS_mount(root_.string().c_str(), nullptr, 1), 0);
        ASSERT_NE(PHYSFS_setWriteDir(root_.string().c_str()), 0);
        Asset::Scene empty;
        PhysOutputStream file("empty.vescene");
        ASSERT_TRUE(empty.SerializeToZeroCopyStream(&file));
    }

    void TearDown() override {
        Framework::Resources::clear();
        EXPECT_NE(PHYSFS_deinit(), 0);
        std::error_code error;
        std::filesystem::remove_all(root_, error);
    }

protected:
    std::filesystem::path root_;
};

using FrameworkAsyncLoad = FrameworkLifecycle;

TEST_F(FrameworkLifecycle, OwnsGameSceneAndDrivesItsLogicThroughWorldInterface) {
    int destructions = 0;
    const auto factory = [&destructions]() -> std::unique_ptr<World::Scene> {
        return std::make_unique<GameScene>(destructions);
    };
    {
        Base framework;
        framework.load("empty.vescene", factory);
        framework.waitForLoad();
        auto& scene = framework.scene();
        framework.update({});
        EXPECT_EQ(scene.get<GameProgress>(scene.player()).ticks, 0);
        framework.play();
        framework.update({});
        EXPECT_EQ(scene.get<GameProgress>(scene.player()).ticks, 1);
        framework.pause();
        framework.update({});
        EXPECT_EQ(scene.get<GameProgress>(scene.player()).ticks, 1);
    }
    EXPECT_EQ(destructions, 1);

    Framework::Server server;
    server.load("empty.vescene", factory);
    server.waitForLoad();
    server.play();
    server.update({});
    EXPECT_EQ(server.scene().get<GameProgress>(server.scene().player()).ticks, 1);
    server.unload();
    EXPECT_EQ(destructions, 2);
}

class Server final : public Framework::Server {
public:
    bool failLoad = false;
    std::vector<int> order;

protected:
    async::task<void> onLoadScene(const World::Scene& scene) override {
        return Base::onLoadScene(scene).then(scheduler(), [this]() {
            if (failLoad) {
                throw std::runtime_error("Load failure");
            }
        });
    }

    void onNetworkUpdate(Clock::duration) override {
        order.push_back(1);
    }

    void onAfterUpdate(Clock::duration) override {
        order.push_back(3);
    }
};

TEST_F(FrameworkLifecycle, StateGuardsAndUpdateOrder) {
    Server server;
    EXPECT_EQ(server.state(), State::Idle);
    EXPECT_NO_THROW(server.update({}));
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    EXPECT_DEATH(server.update(-std::chrono::milliseconds(1)), "[Aa]ssert");
    EXPECT_DEATH(server.load("empty.vescene", {}), "[Aa]ssert");
#endif
    EXPECT_THROW(server.scene(), std::logic_error);
    EXPECT_THROW(server.play(), std::logic_error);
    EXPECT_THROW(server.pause(), std::logic_error);
    EXPECT_THROW(server.synchronizeResources(), std::logic_error);
    server.load("empty.vescene", [&] {
        EXPECT_EQ(server.state(), State::Loading);
        EXPECT_THROW(server.unload(), std::logic_error);
        auto scene = std::make_unique<Scene>();
        scene->order = &server.order;
        return scene;
    });
    server.waitForLoad();
    EXPECT_EQ(server.state(), State::Ready);
    EXPECT_THROW(server.load("empty.vescene", [] { return std::make_unique<Scene>(); }), std::logic_error);
    server.update({});
    EXPECT_EQ(server.order, (std::vector<int>{1, 3}));
    server.order.clear();
    server.play();
    server.update(std::chrono::milliseconds(1));
    EXPECT_EQ(server.order, (std::vector<int>{1, 2, 3}));
    server.pause();
    server.order.clear();
    server.update({});
    EXPECT_EQ(server.order, (std::vector<int>{1, 3}));
    EXPECT_EQ(static_cast<Scene&>(server.scene()).updates, 1);
    EXPECT_THROW(server.pause(), std::logic_error);
    server.unload();
    EXPECT_EQ(server.state(), State::Idle);
    EXPECT_NO_THROW(server.unload());
}

TEST_F(FrameworkLifecycle, ContextInjectedToSceneAndSignalsExit) {
    Base framework;
    EXPECT_FALSE(framework.shouldClose());
    World::Context* capturedContext = nullptr;
    framework.load("empty.vescene", [&](World::Context& ctx) {
        capturedContext = &ctx;
        return std::make_unique<Scene>(ctx);
    });
    framework.waitForLoad();
    EXPECT_EQ(capturedContext, static_cast<World::Context*>(&framework));
    EXPECT_FALSE(framework.shouldClose());

    framework.scene().context().requestExit();
    EXPECT_TRUE(framework.shouldClose());
}

TEST_F(FrameworkLifecycle, FactoryAndResourceFailuresPreservePreviousScene) {
    Server server;
    server.load("empty.vescene", [] { return std::unique_ptr<World::Scene>{}; });
    EXPECT_THROW(server.waitForLoad(), std::runtime_error);
    EXPECT_EQ(server.state(), State::Idle);
    bool failFactory = false;
    server.load("empty.vescene", [&]() -> std::unique_ptr<World::Scene> {
        if (failFactory) {
            throw std::runtime_error("Factory failure");
        }
        return std::make_unique<Scene>();
    });
    server.waitForLoad();
    auto* original = &server.scene();
    auto* originalPhysics = &server.physics();
    server.play();
    server.failLoad = true;
    server.synchronizeResources();
    EXPECT_THROW(server.waitForLoad(), std::runtime_error);
    EXPECT_EQ(&server.scene(), original);
    EXPECT_EQ(&server.physics(), originalPhysics);
    EXPECT_EQ(server.state(), State::Playing);
    server.failLoad = false;
    server.synchronizeResources();
    EXPECT_NO_THROW(server.waitForLoad());
}

TEST_F(FrameworkLifecycle, MultipleFrameworksKeepTheirPhysicsAlive) {
    Server first;
    Server second;
    first.load("empty.vescene", [] { return std::make_unique<Scene>(); });
    first.waitForLoad();
    second.load("empty.vescene", [] { return std::make_unique<Scene>(); });
    second.waitForLoad();
    first.unload();
    second.play();
    EXPECT_NO_THROW(second.update(std::chrono::milliseconds(1)));
}

class FrameworkResources : public ::testing::Test {
protected:
    void SetUp() override {
        root_ = std::filesystem::temp_directory_path() /
            ("volcano-framework-" + std::to_string(Clock::now().time_since_epoch().count()));
        std::filesystem::create_directories(root_ / "models");
        ASSERT_NE(PHYSFS_init("FrameworkCheck"), 0);
        ASSERT_NE(PHYSFS_mount(root_.string().c_str(), nullptr, 1), 0);
        ASSERT_NE(PHYSFS_setWriteDir(root_.string().c_str()), 0);
        Asset::Scene empty;
        PhysOutputStream file("empty.vescene");
        ASSERT_TRUE(empty.SerializeToZeroCopyStream(&file));
    }

    void TearDown() override {
        Framework::Resources::clear();
        EXPECT_NE(PHYSFS_deinit(), 0);
        std::error_code error;
        std::filesystem::remove_all(root_, error);
    }

    void writeModel(const std::string& path) {
        Asset::Model model;
        auto* part = model.add_parts();
        part->set_mesh("mesh.vemesh");
        part->set_material("material.vematerial");
        for (float val : {1.0f, 0.0f, 0.0f, 0.0f,
                          0.0f, 1.0f, 0.0f, 0.0f,
                          0.0f, 0.0f, 1.0f, 0.0f,
                          0.0f, 0.0f, 0.0f, 1.0f}) {
            part->add_transform(val);
        }
        PhysOutputStream file(path);
        EXPECT_TRUE(model.SerializeToZeroCopyStream(&file));
    }

    static auto factory(std::filesystem::path source) {
        return [source = std::move(source)] {
            auto scene = std::make_unique<Scene>();
            scene->emplace<World::Model>(scene->create(), source);
            return scene;
        };
    }

protected:
    std::filesystem::path root_;
};

TEST_F(FrameworkResources, ServerReadsDescriptorsResolvesPathsAndReusesCacheOnReset) {
    writeModel("models/test.vemodel");
    Server server;
    server.load("empty.vescene", factory("./models/test.vemodel"));
    server.waitForLoad();
    const auto& model = *Framework::Resources::loadModel("models/test.vemodel");
    ASSERT_EQ(model.parts_size(), 1);
    EXPECT_EQ(model.parts(0).mesh(), "models/mesh.vemesh");
    EXPECT_EQ(model.parts(0).material(), "models/material.vematerial");
    EXPECT_FALSE(std::filesystem::exists(root_ / "models/mesh.vemesh"));
    std::filesystem::remove(root_ / "models/test.vemodel");
    server.play();
    server.synchronizeResources();
    EXPECT_NO_THROW(server.waitForLoad());
    EXPECT_EQ(server.state(), State::Playing);
    EXPECT_EQ(Framework::Resources::loadModel("models/test.vemodel").get(), &model);
}

TEST_F(FrameworkResources, SharesCommittedModelsBetweenFrameworksWithoutGraphicsInitialization) {
    EXPECT_EQ(gl3wProcs, nullptr);
    writeModel("models/shared.vemodel");
    Server first;
    first.load("empty.vescene", factory("models/shared.vemodel"));
    first.waitForLoad();
    const auto* shared = Framework::Resources::loadModel("models/shared.vemodel").get();
    std::filesystem::remove(root_ / "models/shared.vemodel");

    Server second;
    second.load("empty.vescene", factory("./models/shared.vemodel"));
    EXPECT_NO_THROW(second.waitForLoad());
    EXPECT_EQ(Framework::Resources::loadModel("models/shared.vemodel").get(), shared);
    first.unload();
    second.synchronizeResources();
    EXPECT_NO_THROW(second.waitForLoad());
    EXPECT_EQ(Framework::Resources::loadModel("models/shared.vemodel").get(), shared);
    EXPECT_EQ(gl3wProcs, nullptr);
}

TEST_F(FrameworkResources, FailedSynchronizationDiscardsNewResourcesAndCanRetry) {
    Server server;
    server.load("empty.vescene", [] { return std::make_unique<Scene>(); });
    server.waitForLoad();
    writeModel("models/new.vemodel");
    server.scene().emplace<World::Model>(server.scene().create(), "models/new.vemodel");
    server.failLoad = true;
    server.synchronizeResources();
    EXPECT_THROW(server.waitForLoad(), std::runtime_error);

    server.failLoad = false;
    std::filesystem::remove(root_ / "models/new.vemodel");
    try {
        server.synchronizeResources();
        server.waitForLoad();
        FAIL() << "Expected missing resource";
    } catch (const std::runtime_error& error) {
        EXPECT_NE(std::string(error.what()).find("models/new.vemodel"), std::string::npos);
    }
    writeModel("models/new.vemodel");
    server.synchronizeResources();
    EXPECT_NO_THROW(server.waitForLoad());
    EXPECT_NE(Framework::Resources::loadModel("models/new.vemodel"), nullptr);
}

TEST_F(FrameworkResources, RejectsUnsafeAndMissingPathsAndLeavesIdle) {
    Server server;
    for (const auto* path : {"", "../model", "/../model", "/", "C:model", "a\\b", "."}) {
        server.load("empty.vescene", factory(path));
        EXPECT_THROW(server.waitForLoad(), std::invalid_argument);
        EXPECT_EQ(server.state(), State::Idle);
    }
    server.load("empty.vescene", factory("missing.vemodel"));
    EXPECT_THROW(server.waitForLoad(), std::runtime_error);
    EXPECT_EQ(server.state(), State::Idle);
}
}

namespace {
TEST_F(FrameworkLifecycle, PhysicsAccessFollowsSceneLifetime) {
    Base framework;
    EXPECT_THROW(framework.physics(), std::logic_error);
    framework.load("empty.vescene", [] {
        auto scene = std::make_unique<Scene>();
        const auto entity = scene->create();
        scene->emplace<Transform>(entity);
        scene->emplace<World::RigidBody>(entity).setShape(World::RigidBody::Sphere{0.5f});
        return scene;
    });
    framework.waitForLoad();
    const auto entity = *framework.scene().view<Transform>().begin();
    ASSERT_TRUE(framework.physics().contains(entity));
    framework.update(std::chrono::milliseconds(16));
    EXPECT_EQ(framework.scene().get<Transform>(entity).translation().y(), 0.0f);
    framework.play();
    framework.update(std::chrono::milliseconds(16));
    EXPECT_LT(framework.scene().get<Transform>(entity).translation().y(), 0.0f);
    const Eigen::Vector3f position = framework.scene().get<Transform>(entity).translation();
    framework.pause();
    framework.update(std::chrono::milliseconds(16));
    EXPECT_EQ(framework.scene().get<Transform>(entity).translation(), position);
    framework.physics().setGravity(Eigen::Vector3f::Zero());
    const Base& readonly = framework;
    EXPECT_EQ(readonly.physics().gravity(), Eigen::Vector3f::Zero());
    framework.unload();
    EXPECT_THROW(framework.physics(), std::logic_error);
}
}

namespace {
TEST_F(FrameworkLifecycle, InvalidPhysicsDescriptionRollsBackLoad) {
    Base framework;
    bool invalid = true;
    const auto factory = [&] {
        auto scene = std::make_unique<Scene>();
        const auto entity = scene->create();
        scene->emplace<Transform>(entity);
        scene->emplace<World::RigidBody>(entity).setShape(World::RigidBody::Sphere{0.5f});
        if (invalid) {
            scene->get<Transform>(entity).setScale(Eigen::Vector3f::Constant(2));
        }
        return scene;
    };
    framework.load("empty.vescene", factory);
    EXPECT_THROW(framework.waitForLoad(), std::invalid_argument);
    EXPECT_EQ(framework.state(), State::Idle);
    invalid = false;
    framework.load("empty.vescene", factory);
    framework.waitForLoad();
    EXPECT_EQ(framework.state(), State::Ready);
}
}

namespace {
class DeferredLoad final : public Base {
public:
    ~DeferredLoad() override {
        cancelLoad();
        gate.set();
        try {
            waitForLoad();
        } catch (const async::task_canceled&) {
        }
    }

public:
    async::event_task<void> gate;
    int entities = 0;
    int commits = 0;
    int discards = 0;
    bool waiting = false;
    bool sceneFinished = false;
    std::thread::id entityThread;

protected:
    async::task<void> onLoadScene(const World::Scene& scene) override {
        return Base::onLoadScene(scene).then(scheduler(), [this]() {
            waiting = true;
            return gate.get_task();
        }).then(scheduler(), [this]() {
            loading_.checkCancelled();
            sceneFinished = true;
        });
    }

    async::task<void> onLoadEntity(const World::Scene& scene, entt::entity entity) override {
        return Base::onLoadEntity(scene, entity).then(scheduler(), [this]() {
            loading_.checkCancelled();
            entityThread = std::this_thread::get_id();
            ++entities;
        });
    }

    void onCommitResources() noexcept override {
        ++commits;
    }

    void onDiscardResources() noexcept override {
        ++discards;
    }
};

void pumpUntilWaiting(DeferredLoad& framework) {
    const auto deadline = Clock::now() + std::chrono::seconds(5);
    while (!framework.waiting && Clock::now() < deadline) {
        framework.update({});
        std::this_thread::yield();
    }
    ASSERT_TRUE(framework.waiting);
}

TEST_F(FrameworkAsyncLoad, ComposesSceneAndEntityTasksAndReportsProgressOnOwnerThread) {
    DeferredLoad framework;
    bool created = false;
    framework.load("empty.vescene", [&] {
        created = true;
        auto scene = std::make_unique<Scene>();
        (void)scene->create();
        (void)scene->create();
        return scene;
    });
    EXPECT_FALSE(created);
    EXPECT_EQ(framework.state(), State::Loading);
    pumpUntilWaiting(framework);
    ASSERT_EQ(framework.entities, 2);
    EXPECT_EQ(framework.entityThread, std::this_thread::get_id());
    EXPECT_EQ(framework.loadingProgress().completed, 2);
    EXPECT_EQ(framework.loadingProgress().total, 3);
    EXPECT_GT(framework.loadingProgress().percent, 0);
    EXPECT_LT(framework.loadingProgress().percent, 100);
    EXPECT_FALSE(framework.loadingProgress().content.empty());
    EXPECT_FALSE(framework.sceneFinished);
    EXPECT_EQ(framework.state(), State::Loading);
    framework.gate.set();
    framework.waitForLoad();
    EXPECT_TRUE(framework.sceneFinished);
    EXPECT_EQ(framework.commits, 1);
    EXPECT_EQ(framework.discards, 0);
    EXPECT_EQ(framework.state(), State::Ready);
    EXPECT_EQ(framework.loadingProgress().percent, 100);
}

TEST_F(FrameworkAsyncLoad, CancellationWaitsForChildrenThenRollsBack) {
    DeferredLoad framework;
    framework.load("empty.vescene", [] {
        auto scene = std::make_unique<Scene>();
        (void)scene->create();
        return scene;
    });
    pumpUntilWaiting(framework);
    std::thread cancel([&] { framework.cancelLoad(); });
    cancel.join();
    EXPECT_TRUE(framework.loadCancelled());
    framework.update({});
    EXPECT_EQ(framework.state(), State::Loading);
    framework.gate.set();
    EXPECT_THROW(framework.waitForLoad(), async::task_canceled);
    EXPECT_EQ(framework.state(), State::Idle);
    EXPECT_EQ(framework.commits, 0);
    EXPECT_EQ(framework.discards, 1);
    EXPECT_LT(framework.loadingProgress().percent, 100);
}

TEST_F(FrameworkAsyncLoad, CancelBeforeStartSkipsFactoryAndAllowsRetry) {
    Base framework;
    bool created = false;
    framework.load("empty.vescene", [&] {
        created = true;
        return std::make_unique<Scene>();
    });
    framework.cancelLoad();
    EXPECT_THROW(framework.waitForLoad(), async::task_canceled);
    EXPECT_FALSE(created);
    EXPECT_EQ(framework.state(), State::Idle);
    framework.load("empty.vescene", [] { return std::make_unique<Scene>(); });
    EXPECT_NO_THROW(framework.waitForLoad());
    EXPECT_FALSE(framework.loadCancelled());
}

TEST_F(FrameworkAsyncLoad, CancelledSynchronizeKeepsPlayingSceneAndPhysics) {
    Base framework;
    framework.load("empty.vescene", [] { return std::make_unique<Scene>(); });
    framework.waitForLoad();
    framework.play();
    const auto* scene = &framework.scene();
    const auto* physics = &framework.physics();
    framework.synchronizeResources();
    framework.cancelLoad();
    EXPECT_THROW(framework.waitForLoad(), async::task_canceled);
    EXPECT_EQ(framework.state(), State::Playing);
    EXPECT_EQ(&framework.scene(), scene);
    EXPECT_EQ(&framework.physics(), physics);
    EXPECT_NO_THROW(framework.update({}));
}

TEST_F(FrameworkAsyncLoad, DestructionSettlesOutstandingTopLevelTask) {
    bool created = false;
    {
        Base framework;
        framework.load("empty.vescene", [&] {
            created = true;
            return std::make_unique<Scene>();
        });
    }
    EXPECT_FALSE(created);
}

TEST_F(FrameworkResources, CancelsAfterPreparationWithoutPublishingModels) {
    writeModel("models/cancelled.vemodel");
    DeferredLoad framework;
    framework.load("empty.vescene", factory("models/cancelled.vemodel"));
    pumpUntilWaiting(framework);
    framework.cancelLoad();
    framework.gate.set();
    EXPECT_THROW(framework.waitForLoad(), async::task_canceled);
    std::filesystem::remove(root_ / "models/cancelled.vemodel");
    Base retry;
    retry.load("empty.vescene", factory("models/cancelled.vemodel"));
    EXPECT_THROW(retry.waitForLoad(), std::runtime_error);
    writeModel("models/cancelled.vemodel");
    retry.load("empty.vescene", factory("models/cancelled.vemodel"));
    EXPECT_NO_THROW(retry.waitForLoad());
}
}

namespace {
class ThrowingSceneLoad final : public Base {
public:
    ~ThrowingSceneLoad() override {
        cancelLoad();
        gate.set();
        try {
            waitForLoad();
        } catch (const async::task_canceled&) {
        } catch (const std::runtime_error&) {
        }
    }

public:
    async::event_task<void> gate;
    bool entityStarted = false;
    bool entityFinished = false;
    bool discarded = false;

protected:
    async::task<void> onLoadScene(const World::Scene& scene) override {
        return Base::onLoadScene(scene).then(scheduler(), [](async::task<void> parent) {
            parent.get();
            throw std::runtime_error("Scene hook failed after starting parent tasks");
        });
    }

    async::task<void> onLoadEntity(const World::Scene&, entt::entity) override {
        entityStarted = true;
        return gate.get_task().then(scheduler(), [this]() {
            entityFinished = true;
        });
    }

    void onDiscardResources() noexcept override {
        EXPECT_TRUE(entityFinished);
        discarded = true;
    }
};

TEST_F(FrameworkAsyncLoad, ThrowingSceneHookStillWaitsForStartedEntityTasks) {
    ThrowingSceneLoad framework;
    framework.load("empty.vescene", [] {
        auto scene = std::make_unique<Scene>();
        (void)scene->create();
        return scene;
    });
    const auto deadline = Clock::now() + std::chrono::seconds(5);
    while (!framework.entityStarted && Clock::now() < deadline) {
        framework.processTasks();
        std::this_thread::yield();
    }
    EXPECT_TRUE(framework.entityStarted);
    EXPECT_FALSE(framework.discarded);
    EXPECT_EQ(framework.state(), State::Loading);
    framework.gate.set();
    EXPECT_THROW(framework.waitForLoad(), std::runtime_error);
    EXPECT_TRUE(framework.entityFinished);
    EXPECT_TRUE(framework.discarded);
    EXPECT_EQ(framework.state(), State::Idle);
}
}

namespace {
TEST_F(FrameworkResources, ReleasesModelsAfterTheLastSceneOwnerUnloads) {
    writeModel("models/owned.vemodel");
    std::weak_ptr<Asset::Model> weak;
    {
        Base first;
        Base second;
        first.load("empty.vescene", factory("models/owned.vemodel"));
        first.waitForLoad();
        second.load("empty.vescene", factory("models/owned.vemodel"));
        second.waitForLoad();
        weak = Framework::Resources::loadModel("models/owned.vemodel");
        first.unload();
        EXPECT_FALSE(weak.expired());
    }
    EXPECT_TRUE(weak.expired());
}
}

namespace {
TEST_F(FrameworkResources, LoadsBuiltInResourcesDirectlyWithoutFactories) {
    writeModel("models/direct.vemodel");
    {
        Asset::Material source;
        source.set_albedo_texture("albedo.vetexture");
        PhysOutputStream file("models/material.vematerial");
        EXPECT_TRUE(source.SerializeToZeroCopyStream(&file));
    }
    auto model = Framework::Resources::loadModel("models/direct.vemodel");
    auto material = Framework::Resources::loadMaterial("models/material.vematerial");
    ASSERT_EQ(model->parts_size(), 1);
    EXPECT_EQ(model->parts(0).mesh(), "models/mesh.vemesh");
    EXPECT_EQ(model->parts(0).material(), "models/material.vematerial");
    EXPECT_EQ(material->albedo_texture(), "models/albedo.vetexture");
    EXPECT_EQ(Framework::Resources::loadModel("./models/direct.vemodel"), model);
    std::filesystem::remove(root_ / "models/direct.vemodel");
    EXPECT_EQ(Framework::Resources::loadModel("./models/direct.vemodel"), model);
    std::weak_ptr<Asset::Model> weak = model;
    model.reset();
    EXPECT_TRUE(weak.expired());
}

TEST_F(FrameworkResources, DefaultLoaderValidatesPathsAndReportsFileFailures) {
    for (const auto* path : {"", "../model", "/../model", "/", "C:model", "a\\b", "."}) {
        EXPECT_THROW(Framework::Resources::loadModel(path), std::invalid_argument);
    }
    try {
        Framework::Resources::loadModel("models/missing.vemodel");
        FAIL() << "Expected missing resource";
    } catch (const std::runtime_error& error) {
        EXPECT_NE(std::string(error.what()).find("models/missing.vemodel"), std::string::npos);
    }
    {
        PhysOutputStream file("models/broken.vemodel");
        const ByteArray broken = {0, 1, 2, 3};
        file.writeAll(broken);
    }
    EXPECT_THROW(Framework::Resources::loadModel("models/broken.vemodel"), std::runtime_error);
    writeModel("models/broken.vemodel");
    EXPECT_NO_THROW(Framework::Resources::loadModel("models/broken.vemodel"));
}

TEST_F(FrameworkResources, LoadsSceneManifestWithFactoryAndResolvesRelativeResources) {
    std::filesystem::create_directories(root_ / "scenes/models");
    writeModel("scenes/models/hero.vemodel");
    {
        Asset::Scene sceneAsset;
        sceneAsset.set_name("LevelOne");
        sceneAsset.set_description("Opening level");
        sceneAsset.mutable_gravity()->set_y(-4.5f);

        auto* modelEntity = sceneAsset.add_entities();
        modelEntity->set_name("hero");
        modelEntity->mutable_transform()->mutable_translation()->set_x(2.0f);
        modelEntity->mutable_transform()->mutable_translation()->set_y(3.0f);
        modelEntity->mutable_transform()->mutable_translation()->set_z(4.0f);
        modelEntity->mutable_model()->set_source("models/hero.vemodel");
        auto* matOverride = modelEntity->mutable_model()->add_materials();
        matOverride->set_part(0);
        matOverride->set_albedo_texture("textures/hero.vetexture");
        modelEntity->mutable_rigid_body()->set_motion(Asset::MOTION_DYNAMIC);
        modelEntity->mutable_rigid_body()->mutable_sphere()->set_radius(0.75f);
        modelEntity->mutable_rigid_body()->set_mass(2.0f);

        auto* cameraEntity = sceneAsset.add_entities();
        cameraEntity->mutable_perspective_camera()->set_fov(75.0f);
        cameraEntity->mutable_perspective_camera()->set_aspect(16.0f / 9.0f);
        cameraEntity->mutable_perspective_camera()->set_znear(0.2f);
        cameraEntity->mutable_perspective_camera()->set_zfar(500.0f);

        auto* lightEntity = sceneAsset.add_entities();
        lightEntity->mutable_point_light()->mutable_color()->set_x(1.0f);
        lightEntity->mutable_point_light()->set_strength(2.5f);
        lightEntity->mutable_point_light()->set_range(25.0f);

        std::filesystem::create_directories(root_ / "scenes");
        PhysOutputStream file("scenes/level.vescene");
        ASSERT_TRUE(sceneAsset.SerializeToZeroCopyStream(&file));
    }

    Server server;
    server.load("scenes/level.vescene", [] { return std::make_unique<Scene>(); });
    server.waitForLoad();

    EXPECT_EQ(server.state(), State::Ready);
    EXPECT_EQ(server.scene().name(), "LevelOne");
    EXPECT_EQ(server.scene().description(), "Opening level");

    auto modelView = server.scene().view<World::Inherent, Transform, World::Model>();
    ASSERT_NE(modelView.begin(), modelView.end());
    const auto hero = *modelView.begin();
    EXPECT_EQ(modelView.get<World::Inherent>(hero).name(), "hero");
    EXPECT_EQ(modelView.get<Transform>(hero).translation(), Eigen::Vector3f(2.0f, 3.0f, 4.0f));
    EXPECT_EQ(modelView.get<World::Model>(hero).source(), std::filesystem::path("scenes/models/hero.vemodel"));
    const auto* overrideMat = modelView.get<World::Model>(hero).findMaterial(0);
    ASSERT_NE(overrideMat, nullptr);
    EXPECT_EQ(overrideMat->textures().at(World::Material::Slot::Albedo)->source(),
              std::filesystem::path("scenes/textures/hero.vetexture"));

    auto camView = server.scene().view<World::PerspectiveCamera>();
    ASSERT_NE(camView.begin(), camView.end());
    EXPECT_FLOAT_EQ(camView.get<World::PerspectiveCamera>(*camView.begin()).fov(), 75.0f);

    auto lightView = server.scene().view<World::PointLight>();
    ASSERT_NE(lightView.begin(), lightView.end());
    EXPECT_FLOAT_EQ(lightView.get<World::PointLight>(*lightView.begin()).range(), 25.0f);

    EXPECT_EQ(server.physics().gravity(), Eigen::Vector3f(0.0f, -4.5f, 0.0f));
    EXPECT_TRUE(server.physics().contains(hero));
    const auto& body = server.scene().get<World::RigidBody>(hero);
    EXPECT_EQ(body.motion(), World::RigidBody::Motion::Dynamic);
    EXPECT_FLOAT_EQ(body.mass(), 2.0f);
    ASSERT_TRUE(std::holds_alternative<World::RigidBody::Sphere>(body.shape()));
    EXPECT_FLOAT_EQ(std::get<World::RigidBody::Sphere>(body.shape()).radius(), 0.75f);
}

TEST_F(FrameworkResources, RejectsInvalidAndMissingSceneManifestPaths) {
    Server server;
    for (const auto* path : {"", "../scene.vescene", "scenes/missing.vescene"}) {
        server.load(path, [] { return std::make_unique<Scene>(); });
        EXPECT_THROW(server.waitForLoad(), std::exception);
        EXPECT_EQ(server.state(), State::Idle);
    }
}
}

namespace {
TEST_F(FrameworkResources, SynchronizationReleasesModelHandlesNoLongerUsedByScene) {
    writeModel("models/removed.vemodel");
    Server server;
    server.load("empty.vescene", factory("models/removed.vemodel"));
    server.waitForLoad();
    std::weak_ptr<Asset::Model> model = Framework::Resources::loadModel("models/removed.vemodel");
    ASSERT_FALSE(model.expired());

    server.scene().clear<World::Model>();
    server.synchronizeResources();
    server.waitForLoad();

    EXPECT_TRUE(model.expired());
}

TEST(FrameworkDrawing, ValidatesParametersAndAccessors) {
    Framework::Drawing drawing("canvas", Size{64, 64}, "painter");
    EXPECT_EQ(drawing.name(), "canvas");
    EXPECT_EQ(drawing.size(), (Size{64, 64}));
    EXPECT_EQ(drawing.provider(), "painter");

    drawing.setName("renamed");
    EXPECT_EQ(drawing.name(), "renamed");
    drawing.setSize({128, 64});
    EXPECT_EQ(drawing.size(), (Size{128, 64}));
    drawing.setProvider("other");
    EXPECT_EQ(drawing.provider(), "other");

    drawing.setDependencies({"dep1", "dep2"});
    EXPECT_EQ(drawing.dependencies().size(), 2);
    EXPECT_EQ(drawing.dependencies()[0], "dep1");

    drawing.setFonts({"fonts/test.ttf"});
    EXPECT_EQ(drawing.fonts().size(), 1);
    EXPECT_EQ(drawing.fonts()[0], "fonts/test.ttf");

    EXPECT_THROW(Framework::Drawing("", Size{64, 64}, "p"), std::invalid_argument);
    EXPECT_THROW(Framework::Drawing("d", Size{0, 64}, "p"), std::invalid_argument);
    EXPECT_THROW(Framework::Drawing("d", Size{64, 0}, "p"), std::invalid_argument);
    EXPECT_THROW(drawing.setName(""), std::invalid_argument);
    EXPECT_THROW(drawing.setSize({0, 64}), std::invalid_argument);
    EXPECT_THROW(drawing.setSize({64, -1}), std::invalid_argument);
}
} // namespace
