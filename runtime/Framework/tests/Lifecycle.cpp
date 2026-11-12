#include <atomic>
#include <chrono>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include <GL/gl3w.h>
#include <gtest/gtest.h>
#include <stdexec/execution.hpp>
#include <exec/task.hpp>

#include <Volcano/PhysFile.h>
#include <Volcano/Asset/Common.h>
#include <Volcano/Asset/Mesh.h>
#include <Volcano/Asset/Texture.h>
#include <Volcano/World/Model.h>
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

TEST(FrameworkLifecycle, OwnsGameSceneAndDrivesItsLogicThroughWorldInterface) {
    int destructions = 0;
    const auto factory = [&destructions]() -> std::unique_ptr<World::Scene> {
        return std::make_unique<GameScene>(destructions);
    };
    {
        Base framework;
        framework.load(factory);
        auto& scene = framework.scene();
        framework.update({});
        EXPECT_EQ(scene.get<GameProgress>(scene.player()).ticks, 0);
        framework.play();
        framework.update({});
        EXPECT_EQ(scene.get<GameProgress>(scene.player()).ticks, 1);
        framework.pause();
        framework.update({});
        EXPECT_EQ(scene.get<GameProgress>(scene.player()).ticks, 1);
        framework.stop();
        EXPECT_EQ(destructions, 1);
        auto& reset = framework.scene();
        EXPECT_EQ(reset.get<GameProgress>(reset.player()).ticks, 0);
    }
    EXPECT_EQ(destructions, 2);

    Framework::Server server;
    server.load(factory);
    server.play();
    server.update({});
    EXPECT_EQ(server.scene().get<GameProgress>(server.scene().player()).ticks, 1);
    server.unload();
    EXPECT_EQ(destructions, 3);
}

class Server final : public Framework::Server {
public:
    bool failLoad = false;
    std::vector<int> order;

protected:
    exec::task<void> onLoadScene(const World::Scene& scene) override {
        co_await Base::onLoadScene(scene);
        if (failLoad) {
            throw std::runtime_error("Load failure");
        }
    }

    void onNetworkUpdate(Clock::duration) override {
        order.push_back(1);
    }

    void onAfterUpdate(Clock::duration) override {
        order.push_back(3);
    }
};

TEST(FrameworkLifecycle, StateGuardsAndUpdateOrder) {
    Server server;
    EXPECT_EQ(server.state(), State::Idle);
    EXPECT_NO_THROW(server.update({}));
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    EXPECT_DEATH(server.update(-std::chrono::milliseconds(1)), "[Aa]ssert");
    EXPECT_DEATH(server.scene(), "[Aa]ssert");
    EXPECT_DEATH(server.play(), "[Aa]ssert");
    EXPECT_DEATH(server.pause(), "[Aa]ssert");
    EXPECT_DEATH(server.stop(), "[Aa]ssert");
    EXPECT_DEATH(server.synchronizeResources(), "[Aa]ssert");
    EXPECT_DEATH(server.load({}), "[Aa]ssert");
#endif
    server.load([&] {
        EXPECT_EQ(server.state(), State::Loading);
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
        EXPECT_DEATH(server.unload(), "[Aa]ssert");
#endif
        auto scene = std::make_unique<Scene>();
        scene->order = &server.order;
        return scene;
    });
    EXPECT_EQ(server.state(), State::Ready);
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    EXPECT_DEATH(server.load([] { return std::make_unique<Scene>(); }), "[Aa]ssert");
#endif
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
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    EXPECT_DEATH(server.pause(), "[Aa]ssert");
#endif
    server.play();
    server.stop();
    EXPECT_EQ(server.state(), State::Ready);
    EXPECT_EQ(static_cast<Scene&>(server.scene()).updates, 0);
    server.unload();
    EXPECT_EQ(server.state(), State::Idle);
    EXPECT_NO_THROW(server.unload());
}

TEST(FrameworkLifecycle, ContextInjectedToSceneAndSignalsExit) {
    Base framework;
    EXPECT_FALSE(framework.shouldClose());
    World::Context* capturedContext = nullptr;
    framework.load([&](World::Context& ctx) {
        capturedContext = &ctx;
        return std::make_unique<Scene>(ctx);
    });
    EXPECT_EQ(capturedContext, static_cast<World::Context*>(&framework));
    EXPECT_FALSE(framework.shouldClose());

    framework.scene().context().requestExit();
    EXPECT_TRUE(framework.shouldClose());
}

TEST(FrameworkLifecycle, FactoryAndResourceFailuresPreservePreviousScene) {
    Server server;
    EXPECT_THROW(server.load([] { return std::unique_ptr<World::Scene>{}; }), std::runtime_error);
    EXPECT_EQ(server.state(), State::Idle);
    bool failFactory = false;
    server.load([&]() -> std::unique_ptr<World::Scene> {
        if (failFactory) {
            throw std::runtime_error("Factory failure");
        }
        return std::make_unique<Scene>();
    });
    auto* original = &server.scene();
    auto* originalPhysics = &server.physics();
    server.play();
    failFactory = true;
    EXPECT_THROW(server.stop(), std::runtime_error);
    EXPECT_EQ(&server.scene(), original);
    EXPECT_EQ(&server.physics(), originalPhysics);
    EXPECT_EQ(server.state(), State::Playing);
    failFactory = false;
    server.failLoad = true;
    EXPECT_THROW(server.stop(), std::runtime_error);
    EXPECT_EQ(&server.scene(), original);
    EXPECT_EQ(&server.physics(), originalPhysics);
    EXPECT_THROW(server.synchronizeResources(), std::runtime_error);
    EXPECT_EQ(server.state(), State::Playing);
    server.failLoad = false;
    EXPECT_NO_THROW(server.stop());
}

TEST(FrameworkLifecycle, MultipleFrameworksKeepTheirPhysicsAlive) {
    Server first;
    Server second;
    first.load([] { return std::make_unique<Scene>(); });
    second.load([] { return std::make_unique<Scene>(); });
    first.unload();
    second.play();
    EXPECT_NO_THROW(second.update(std::chrono::milliseconds(1)));
    EXPECT_NO_THROW(second.stop());
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
    server.load(factory("./models/test.vemodel"));
    const auto& model = *Framework::Resources::loadModel("models/test.vemodel");
    ASSERT_EQ(model.parts_size(), 1);
    EXPECT_EQ(model.parts(0).mesh(), "models/mesh.vemesh");
    EXPECT_EQ(model.parts(0).material(), "models/material.vematerial");
    EXPECT_FALSE(std::filesystem::exists(root_ / "models/mesh.vemesh"));
    std::filesystem::remove(root_ / "models/test.vemodel");
    server.play();
    EXPECT_NO_THROW(server.synchronizeResources());
    EXPECT_EQ(server.state(), State::Playing);
    EXPECT_NO_THROW(server.stop());
    EXPECT_EQ(Framework::Resources::loadModel("models/test.vemodel").get(), &model);
}

TEST_F(FrameworkResources, SharesCommittedModelsBetweenFrameworksWithoutGraphicsInitialization) {
    EXPECT_EQ(gl3wProcs, nullptr);
    writeModel("models/shared.vemodel");
    Server first;
    first.load(factory("models/shared.vemodel"));
    const auto* shared = Framework::Resources::loadModel("models/shared.vemodel").get();
    std::filesystem::remove(root_ / "models/shared.vemodel");

    Server second;
    EXPECT_NO_THROW(second.load(factory("./models/shared.vemodel")));
    EXPECT_EQ(Framework::Resources::loadModel("models/shared.vemodel").get(), shared);
    first.unload();
    EXPECT_NO_THROW(second.synchronizeResources());
    EXPECT_EQ(Framework::Resources::loadModel("models/shared.vemodel").get(), shared);
    EXPECT_EQ(gl3wProcs, nullptr);
}

TEST_F(FrameworkResources, FailedSynchronizationDiscardsNewResourcesAndCanRetry) {
    Server server;
    server.load([] { return std::make_unique<Scene>(); });
    writeModel("models/new.vemodel");
    server.scene().emplace<World::Model>(server.scene().create(), "models/new.vemodel");
    server.failLoad = true;
    EXPECT_THROW(server.synchronizeResources(), std::runtime_error);

    server.failLoad = false;
    std::filesystem::remove(root_ / "models/new.vemodel");
    try {
        server.synchronizeResources();
        FAIL() << "Expected missing resource";
    } catch (const std::runtime_error& error) {
        EXPECT_NE(std::string(error.what()).find("models/new.vemodel"), std::string::npos);
    }
    writeModel("models/new.vemodel");
    EXPECT_NO_THROW(server.synchronizeResources());
    EXPECT_NE(Framework::Resources::loadModel("models/new.vemodel"), nullptr);
}

TEST_F(FrameworkResources, RejectsUnsafeAndMissingPathsAndLeavesIdle) {
    Server server;
    for (const auto* path : {"", "../model", "/../model", "/", "C:model", "a\\b", "."}) {
        EXPECT_THROW(server.load(factory(path)), std::invalid_argument);
        EXPECT_EQ(server.state(), State::Idle);
    }
    EXPECT_THROW(server.load(factory("missing.vemodel")), std::runtime_error);
    EXPECT_EQ(server.state(), State::Idle);
}
}

namespace {
TEST(FrameworkLifecycle, PhysicsAccessFollowsSceneLifetime) {
    Base framework;
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    EXPECT_DEATH(framework.physics(), "[Aa]ssert");
#endif
    framework.load([] {
        auto scene = std::make_unique<Scene>();
        const auto entity = scene->create();
        scene->emplace<Transform>(entity);
        scene->emplace<World::RigidBody>(entity).setShape(World::RigidBody::Sphere{0.5f});
        return scene;
    });
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
    framework.stop();
    EXPECT_EQ(framework.physics().gravity(), Eigen::Vector3f(0, -9.81f, 0));
    framework.unload();
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    EXPECT_DEATH(framework.physics(), "[Aa]ssert");
#endif
}
}

namespace {
TEST(FrameworkLifecycle, InvalidPhysicsDescriptionRollsBackLoadAndReset) {
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
    EXPECT_THROW(framework.load(factory), std::invalid_argument);
    EXPECT_EQ(framework.state(), State::Idle);
    invalid = false;
    framework.load(factory);
    auto* original = &framework.scene();
    framework.play();
    invalid = true;
    EXPECT_THROW(framework.stop(), std::invalid_argument);
    EXPECT_EQ(&framework.scene(), original);
    EXPECT_EQ(framework.state(), State::Playing);
    EXPECT_NO_THROW(framework.update(std::chrono::milliseconds(16)));
    invalid = false;
    EXPECT_NO_THROW(framework.stop());
}
}

namespace {
class DeferredLoad final : public Base {
public:
    ~DeferredLoad() override {
        cancelLoad();
        gate.store(true);
        try {
            waitForLoad();
        } catch (const Base::LoadCanceled&) {
        }
    }

public:
    std::atomic<bool> gate{false};
    int entities = 0;
    int commits = 0;
    int discards = 0;
    bool waiting = false;
    bool sceneFinished = false;
    std::thread::id entityThread;

protected:
    exec::task<void> onLoadScene(const World::Scene& scene) override {
        co_await Base::onLoadScene(scene);
        waiting = true;
        while (!gate.load()) {
            checkLoadCancelled();
            co_await stdexec::schedule(scheduler());
        }
        checkLoadCancelled();
        sceneFinished = true;
    }

    exec::task<void> onLoadEntity(const World::Scene& scene, entt::entity entity) override {
        co_await Base::onLoadEntity(scene, entity);
        checkLoadCancelled();
        entityThread = std::this_thread::get_id();
        ++entities;
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

TEST(FrameworkAsyncLoad, ComposesSceneAndEntityTasksAndReportsProgressOnOwnerThread) {
    DeferredLoad framework;
    bool created = false;
    framework.loadAsync([&] {
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
    framework.gate.store(true);
    framework.waitForLoad();
    EXPECT_TRUE(framework.sceneFinished);
    EXPECT_EQ(framework.commits, 1);
    EXPECT_EQ(framework.discards, 0);
    EXPECT_EQ(framework.state(), State::Ready);
    EXPECT_EQ(framework.loadingProgress().percent, 100);
}

TEST(FrameworkAsyncLoad, CancellationWaitsForChildrenThenRollsBack) {
    DeferredLoad framework;
    framework.loadAsync([] {
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
    framework.gate.store(true);
    EXPECT_THROW(framework.waitForLoad(), Base::LoadCanceled);
    EXPECT_EQ(framework.state(), State::Idle);
    EXPECT_EQ(framework.commits, 0);
    EXPECT_EQ(framework.discards, 1);
    EXPECT_LT(framework.loadingProgress().percent, 100);
}

TEST(FrameworkAsyncLoad, CancelBeforeStartSkipsFactoryAndAllowsRetry) {
    Base framework;
    bool created = false;
    framework.loadAsync([&] {
        created = true;
        return std::make_unique<Scene>();
    });
    framework.cancelLoad();
    EXPECT_THROW(framework.waitForLoad(), Base::LoadCanceled);
    EXPECT_FALSE(created);
    EXPECT_EQ(framework.state(), State::Idle);
    EXPECT_NO_THROW(framework.load([] { return std::make_unique<Scene>(); }));
    EXPECT_FALSE(framework.loadCancelled());
}

TEST(FrameworkAsyncLoad, CancelledResetKeepsPlayingSceneAndPhysics) {
    Base framework;
    framework.load([] { return std::make_unique<Scene>(); });
    framework.play();
    const auto* scene = &framework.scene();
    const auto* physics = &framework.physics();
    framework.stopAsync();
    framework.cancelLoad();
    EXPECT_THROW(framework.waitForLoad(), Base::LoadCanceled);
    EXPECT_EQ(framework.state(), State::Playing);
    EXPECT_EQ(&framework.scene(), scene);
    EXPECT_EQ(&framework.physics(), physics);
    EXPECT_NO_THROW(framework.update({}));
}

TEST(FrameworkAsyncLoad, DestructionSettlesOutstandingTopLevelTask) {
    bool created = false;
    {
        Base framework;
        framework.loadAsync([&] {
            created = true;
            return std::make_unique<Scene>();
        });
    }
    EXPECT_FALSE(created);
}

TEST_F(FrameworkResources, CancelsAfterPreparationWithoutPublishingModels) {
    writeModel("models/cancelled.vemodel");
    DeferredLoad framework;
    framework.loadAsync(factory("models/cancelled.vemodel"));
    pumpUntilWaiting(framework);
    framework.cancelLoad();
    framework.gate.store(true);
    EXPECT_THROW(framework.waitForLoad(), Base::LoadCanceled);
    std::filesystem::remove(root_ / "models/cancelled.vemodel");
    Base retry;
    EXPECT_THROW(retry.load(factory("models/cancelled.vemodel")), std::runtime_error);
    writeModel("models/cancelled.vemodel");
    EXPECT_NO_THROW(retry.load(factory("models/cancelled.vemodel")));
}
}

namespace {
class ThrowingSceneLoad final : public Base {
public:
    ~ThrowingSceneLoad() override {
        cancelLoad();
        gate.store(true);
        try {
            waitForLoad();
        } catch (const std::runtime_error&) {
        }
    }

public:
    std::atomic<bool> gate{false};
    bool entityStarted = false;
    bool entityFinished = false;
    bool discarded = false;

protected:
    exec::task<void> onLoadScene(const World::Scene& scene) override {
        co_await Base::onLoadScene(scene);
        throw std::runtime_error("Scene hook failed after starting parent tasks");
    }

    exec::task<void> onLoadEntity(const World::Scene&, entt::entity) override {
        entityStarted = true;
        while (!gate.load()) {
            co_await stdexec::schedule(scheduler());
        }
        entityFinished = true;
    }

    void onDiscardResources() noexcept override {
        EXPECT_TRUE(entityFinished);
        discarded = true;
    }
};

TEST(FrameworkAsyncLoad, ThrowingSceneHookStillWaitsForStartedEntityTasks) {
    ThrowingSceneLoad framework;
    framework.loadAsync([] {
        auto scene = std::make_unique<Scene>();
        (void)scene->create();
        return scene;
    });
    framework.processTasks();
    EXPECT_TRUE(framework.entityStarted);
    EXPECT_FALSE(framework.discarded);
    EXPECT_EQ(framework.state(), State::Loading);
    framework.gate.store(true);
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
        first.load(factory("models/owned.vemodel"));
        second.load(factory("models/owned.vemodel"));
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
}

namespace {
TEST_F(FrameworkResources, SynchronizationReleasesModelHandlesNoLongerUsedByScene) {
    writeModel("models/removed.vemodel");
    Server server;
    server.load(factory("models/removed.vemodel"));
    std::weak_ptr<Asset::Model> model = Framework::Resources::loadModel("models/removed.vemodel");
    ASSERT_FALSE(model.expired());

    server.scene().clear<World::Model>();
    server.synchronizeResources();

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
