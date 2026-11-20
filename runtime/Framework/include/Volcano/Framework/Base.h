#ifndef VOLCANO_FRAMEWORK_BASE_H
#define VOLCANO_FRAMEWORK_BASE_H

#include <cstddef>
#include <exception>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include <async++.h>
#include <entt/entity/entity.hpp>

#include <Volcano/Asset/Common.h>
#include <Volcano/Physics/System.h>
#include <Volcano/World/Context.h>
#include <Volcano/World/Scene.h>
#include <Volcano/Framework/Common.h>

VOLCANO_FRAMEWORK_BEGIN

// Owns the game Scene; game rules remain in the Scene subclass.
class Base : public World::Context {
public:
    enum class State { Idle, Loading, Ready, Playing, Paused };

    struct LoadProgress {
        float percent = 0;
        std::string content;
        std::size_t completed = 0;
        std::size_t total = 0;
    };

    using SceneFactory = std::function<std::unique_ptr<World::Scene>(World::Context&)>;

    Base();
    virtual ~Base();
    Base(const Base&) = delete;
    Base& operator=(const Base&) = delete;
    Base(Base&&) = delete;
    Base& operator=(Base&&) = delete;

public:
    void load(std::filesystem::path scenePath, SceneFactory factory);

    template <typename F>
        requires (!std::is_same_v<std::decay_t<F>, SceneFactory> && std::invocable<F>)
    void load(std::filesystem::path scenePath, F&& factory) {
        if constexpr (std::invocable<F, World::Context&>) {
            load(std::move(scenePath), SceneFactory{std::forward<F>(factory)});
        } else {
            load(std::move(scenePath), SceneFactory{[f = std::forward<F>(factory)](World::Context&) {
                return f();
            }});
        }
    }

    void play();
    void pause();
    void unload();
    void synchronizeResources();
    void update(Clock::duration elapsed);

    void requestExit() override;
    virtual bool shouldClose() const noexcept;

    void processTasks();
    void waitForLoad();
    // May be called from another thread; cancellation takes effect at the next interruption point.
    void cancelLoad() noexcept;

    bool loadCancelled() const noexcept {
        return loading_.cancelled();
    }

    void schedule(async::task_run_handle task);

    Base& scheduler() noexcept {
        return *this;
    }

    const LoadProgress& loadingProgress() const noexcept {
        return loading_.progress;
    }

    State state() const noexcept {
        return state_;
    }

    World::Scene& scene();
    const World::Scene& scene() const;
    Physics::System& physics();
    const Physics::System& physics() const;

protected:
    struct LoadingState {
    public:
        void cancel() noexcept;
        bool cancelled() const noexcept;
        void checkCancelled() const;
        void rethrowIfError();
        void setContent(std::string content);
        void collectEntities(const World::Scene& candidate);
        void completeEntity(entt::entity entity);
        void beginCommit();
        void completeCommit() noexcept;
        void discard() noexcept;
        void reset() noexcept;

        static void populateScene(World::Scene& target, const Asset::Scene& source);

    public:
        async::cancellation_token cancellation;
        std::exception_ptr error;
        LoadProgress progress;
        std::map<entt::entity, std::string> entities;
        std::map<std::filesystem::path, std::shared_ptr<Asset::Model>> models;
        std::unique_ptr<World::Scene> scene;
        std::unique_ptr<Physics::System> physics;
        SceneFactory factory;
    };

protected:
    virtual async::task<void> onLoadScene(const World::Scene& scene);
    virtual async::task<void> onLoadEntity(const World::Scene& scene, entt::entity entity);
    virtual void onBeforeTasks();
    const Asset::Model& model(const std::filesystem::path& path) const;
    World::Scene* currentScene() noexcept;
    virtual void onUnloadResources() noexcept;
    virtual void onCommitResources() noexcept;
    virtual void onDiscardResources() noexcept;

    virtual void onBeforeUpdate(Clock::duration elapsed);
    virtual void onAfterUpdate(Clock::duration elapsed);

protected:
    LoadingState loading_;

private:
    void beginLoad(std::filesystem::path scenePath, SceneFactory factory);

private:
    async::fifo_scheduler scheduler_;
    const std::thread::id owner_thread_ = std::this_thread::get_id();
    std::map<std::filesystem::path, std::shared_ptr<Asset::Model>> models_;
    bool should_close_ = false;
    State state_ = State::Idle;
    std::unique_ptr<World::Scene> scene_;
    std::unique_ptr<Physics::System> physics_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_BASE_H
