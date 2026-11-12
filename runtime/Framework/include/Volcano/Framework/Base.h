#ifndef VOLCANO_FRAMEWORK_BASE_H
#define VOLCANO_FRAMEWORK_BASE_H

#include <cstddef>
#include <exception>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include <entt/entity/entity.hpp>
#include <stdexec/execution.hpp>
#include <exec/task.hpp>

#include <Volcano/FifoScheduler.h>
#include <Volcano/Asset/Common.h>
#include <Volcano/Physics/System.h>
#include <Volcano/World/Context.h>
#include <Volcano/World/Scene.h>
#include <Volcano/Framework/Common.h>

VOLCANO_FRAMEWORK_BEGIN

// Owns the game Scene; game rules remain in the Scene subclass.
class Base : public World::Context {
public:
    struct LoadCanceled : std::runtime_error {
        LoadCanceled() : std::runtime_error("Load canceled") {}
    };

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

    void loadAsync(SceneFactory factory);

    template <typename F>
        requires (!std::is_same_v<std::decay_t<F>, SceneFactory> && std::invocable<F>)
    void loadAsync(F&& factory) {
        if constexpr (std::invocable<F, World::Context&>) {
            loadAsync(SceneFactory{std::forward<F>(factory)});
        } else {
            loadAsync(SceneFactory{[f = std::forward<F>(factory)](World::Context&) {
                return f();
            }});
        }
    }

    void stopAsync();
    void synchronizeResourcesAsync();
    // Synchronous conveniences pump the same queue and propagate the final task's exception.
    void load(SceneFactory factory);

    template <typename F>
        requires (!std::is_same_v<std::decay_t<F>, SceneFactory> && std::invocable<F>)
    void load(F&& factory) {
        if constexpr (std::invocable<F, World::Context&>) {
            load(SceneFactory{std::forward<F>(factory)});
        } else {
            load(SceneFactory{[f = std::forward<F>(factory)](World::Context&) {
                return f();
            }});
        }
    }

    void synchronizeResources();
    void play();
    void pause();
    void stop();
    void unload();
    void update(Clock::duration elapsed);

    void requestExit() override;
    virtual bool shouldClose() const noexcept;

    void processTasks();
    void waitForLoad();
    // May be called from another thread; cancellation takes effect at the next interruption point.
    void cancelLoad() noexcept;

    bool loadCancelled() const noexcept {
        return cancellation_ && cancellation_->stop_requested();
    }

    FifoScheduler::Scheduler scheduler() noexcept {
        return scheduler_.get_scheduler();
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
    virtual exec::task<void> onLoadScene(const World::Scene& scene);
    virtual exec::task<void> onLoadEntity(const World::Scene& scene, entt::entity entity);
    virtual void onBeforeTasks();
    void checkLoadCancelled() const;
    void setLoadingContent(std::string content);
    const Asset::Model& model(const std::filesystem::path& path) const;
    virtual void onUnloadResources() noexcept;
    virtual void onCommitResources() noexcept;
    virtual void onDiscardResources() noexcept;

    virtual void onBeforeUpdate(Clock::duration elapsed);
    virtual void onAfterUpdate(Clock::duration elapsed);

private:
    void beginLoad(SceneFactory factory);
    void drainLoad();
    exec::task<void> loadCoroutine(bool replace, State previous);

private:
    FifoScheduler scheduler_;
    const std::thread::id owner_thread_ = std::this_thread::get_id();
    std::optional<stdexec::inplace_stop_source> cancellation_;
    std::exception_ptr last_error_{};
    LoadProgress progress_;
    std::map<entt::entity, std::string> loading_entities_;
    std::map<std::filesystem::path, std::shared_ptr<Asset::Model>> models_;
    std::map<std::filesystem::path, std::shared_ptr<Asset::Model>> pending_models_;
    std::unique_ptr<World::Scene> pending_scene_;
    std::unique_ptr<Physics::System> pending_physics_;
    SceneFactory scene_factory_;
    SceneFactory pending_factory_;
    bool should_close_ = false;
    State state_ = State::Idle;
    std::unique_ptr<World::Scene> scene_;
    std::unique_ptr<Physics::System> physics_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_BASE_H
