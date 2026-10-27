#include <chrono>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <Volcano/Framework/Server.h>
#include <Volcano/PhysFile.h>
#include <Volcano/Schema/Model.h>
#include <Volcano/World/Model.h>

namespace {
using namespace Volcano;
using Framework::Base;
using State = Base::State;

class Scene final : public World::Scene {
public:
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
    using Base::modelResource;
    bool failLoad = false;
    std::vector<int> order;

protected:
    void onLoadResources(const World::Scene& scene) override {
        Base::onLoadResources(scene);
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
    EXPECT_THROW(server.update(-std::chrono::milliseconds(1)), std::invalid_argument);
    EXPECT_THROW(server.scene(), std::logic_error);
    EXPECT_THROW(server.play(), std::logic_error);
    EXPECT_THROW(server.pause(), std::logic_error);
    EXPECT_THROW(server.stop(), std::logic_error);
    EXPECT_THROW(server.synchronizeResources(), std::logic_error);
    EXPECT_THROW(server.load({}), std::invalid_argument);
    server.load([&] {
        EXPECT_EQ(server.state(), State::Loading);
        EXPECT_THROW(server.unload(), std::logic_error);
        EXPECT_THROW(server.update({}), std::logic_error);
        auto scene = std::make_unique<Scene>();
        scene->order = &server.order;
        return scene;
    });
    EXPECT_EQ(server.state(), State::Ready);
    EXPECT_THROW(server.load([] { return std::make_unique<Scene>(); }), std::logic_error);
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
    server.play();
    server.stop();
    EXPECT_EQ(server.state(), State::Ready);
    EXPECT_EQ(static_cast<Scene&>(server.scene()).updates, 0);
    server.unload();
    EXPECT_EQ(server.state(), State::Idle);
    EXPECT_NO_THROW(server.unload());
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
    server.play();
    failFactory = true;
    EXPECT_THROW(server.stop(), std::runtime_error);
    EXPECT_EQ(&server.scene(), original);
    EXPECT_EQ(server.state(), State::Playing);
    failFactory = false;
    server.failLoad = true;
    EXPECT_THROW(server.stop(), std::runtime_error);
    EXPECT_EQ(&server.scene(), original);
    EXPECT_THROW(server.synchronizeResources(), std::runtime_error);
    EXPECT_EQ(server.state(), State::Playing);
    server.failLoad = false;
    EXPECT_NO_THROW(server.stop());
}

TEST(FrameworkLifecycle, MultipleScenesKeepTheirInternalPhysicsAlive) {
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
        EXPECT_NE(PHYSFS_deinit(), 0);
        std::error_code error;
        std::filesystem::remove_all(root_, error);
    }

    void writeModel(const std::string& path) {
        Schema::Model model;
        model.parts.push_back({"mesh.vemesh", "material.vematerial", Eigen::Affine3f::Identity()});
        PhysFile file(path, PhysFile::OpenMode::WriteOnly);
        model.write(file);
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
    const auto& model = server.modelResource("models/test.vemodel");
    ASSERT_EQ(model.parts.size(), 1);
    EXPECT_EQ(model.parts[0].mesh, "models/mesh.vemesh");
    EXPECT_EQ(model.parts[0].material, "models/material.vematerial");
    EXPECT_FALSE(std::filesystem::exists(root_ / "models/mesh.vemesh"));
    std::filesystem::remove(root_ / "models/test.vemodel");
    server.play();
    EXPECT_NO_THROW(server.synchronizeResources());
    EXPECT_EQ(server.state(), State::Playing);
    EXPECT_NO_THROW(server.stop());
    EXPECT_EQ(&server.modelResource("models/test.vemodel"), &model);
}

TEST_F(FrameworkResources, FailedSynchronizationDiscardsNewResourcesAndCanRetry) {
    Server server;
    server.load([] { return std::make_unique<Scene>(); });
    writeModel("models/new.vemodel");
    server.scene().emplace<World::Model>(server.scene().create(), "models/new.vemodel");
    server.failLoad = true;
    EXPECT_THROW(server.synchronizeResources(), std::runtime_error);
    EXPECT_THROW(server.modelResource("models/new.vemodel"), std::logic_error);
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
    EXPECT_NO_THROW(server.modelResource("models/new.vemodel"));
}

TEST_F(FrameworkResources, RejectsUnsafeAndMissingPathsAndLeavesIdle) {
    Server server;
    for (const auto* path : {"", "../model", "/model", "C:model", "a\\b", "."}) {
        EXPECT_THROW(server.load(factory(path)), std::invalid_argument);
        EXPECT_EQ(server.state(), State::Idle);
    }
    EXPECT_THROW(server.load(factory("missing.vemodel")), std::runtime_error);
    EXPECT_EQ(server.state(), State::Idle);
}
}
