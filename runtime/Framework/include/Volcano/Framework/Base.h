#ifndef VOLCANO_FRAMEWORK_BASE_H
#define VOLCANO_FRAMEWORK_BASE_H

#include <cstddef>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <async++.h>
#include <entt/entity/entity.hpp>

#include <Volcano/Physics/System.h>
#include <Volcano/Schema/Model.h>
#include <Volcano/World/Scene.h>
#include <Volcano/Framework/Common.h>

VOLCANO_FRAMEWORK_BEGIN

// Owns the game Scene; game rules remain in the Scene subclass.
class Base {
public:
    enum class State { Idle, Loading, Ready, Playing, Paused };

    struct LoadProgress {
        float percent = 0;
        std::string content;
        std::size_t completed = 0;
        std::size_t total = 0;
    };

    using SceneFactory = std::function<std::unique_ptr<World::Scene>()>;

    Base();
    virtual ~Base();
    Base(const Base&) = delete;
    Base& operator=(const Base&) = delete;
    Base(Base&&) = delete;
    Base& operator=(Base&&) = delete;

    async::task<void> loadAsync(SceneFactory factory);
    async::task<void> stopAsync();
    async::task<void> synchronizeResourcesAsync();
    // Synchronous conveniences pump the same queue and propagate the final task's exception.
    void load(SceneFactory factory);
    void synchronizeResources();
    void play();
    void pause();
    void stop();
    void unload();
    void update(Clock::duration elapsed);

    void processTasks();
    void waitForLoad();
    // May be called from another thread; cancellation takes effect at the next interruption point.
    void cancelLoad() noexcept;

    bool loadCancelled() const noexcept {
        return cancellation_.is_canceled();
    }

    async::fifo_scheduler& scheduler() noexcept {
        return scheduler_;
    }

    const LoadProgress& loadingProgress() const noexcept {
        return progress_;
    }

    State state() const noexcept {
        return state_;
    }

    World::Scene& scene();
    const World::Scene& scene() const;
    Physics::System& physics();
    const Physics::System& physics() const;

protected:
    virtual async::task<void> onLoadScene(const World::Scene& scene);
    virtual async::task<void> onLoadEntity(const World::Scene& scene, entt::entity entity);
    virtual void onBeforeTasks();
    void checkLoadCancelled() const;
    void setLoadingContent(std::string content);
    const Schema::Model& model(const std::filesystem::path& path) const;
    virtual void onUnloadResources() noexcept;
    virtual void onCommitResources() noexcept;
    virtual void onDiscardResources() noexcept;

    virtual void onBeforeUpdate(Clock::duration elapsed);
    virtual void onAfterUpdate(Clock::duration elapsed);

private:
    async::task<void> beginLoad(SceneFactory factory);
    void drainLoad();

private:
    async::fifo_scheduler scheduler_;
    const std::thread::id owner_thread_ = std::this_thread::get_id();
    async::shared_task<void> loading_;
    std::vector<async::shared_task<void>> entity_tasks_;
    async::cancellation_token cancellation_;
    LoadProgress progress_;
    std::map<entt::entity, std::string> loading_entities_;
    std::map<std::filesystem::path, std::shared_ptr<Schema::Model>> models_;
    std::map<std::filesystem::path, std::shared_ptr<Schema::Model>> pending_models_;
    std::unique_ptr<World::Scene> pending_scene_;
    std::unique_ptr<Physics::System> pending_physics_;
    SceneFactory scene_factory_;
    SceneFactory pending_factory_;
    State state_ = State::Idle;
    std::unique_ptr<World::Scene> scene_;
    std::unique_ptr<Physics::System> physics_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_BASE_H
