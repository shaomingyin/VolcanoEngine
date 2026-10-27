#ifndef VOLCANO_FRAMEWORK_BASE_H
#define VOLCANO_FRAMEWORK_BASE_H

#include <filesystem>
#include <functional>
#include <map>
#include <memory>

#include <Volcano/World/Scene.h>
#include <Volcano/Schema/Model.h>
#include <Volcano/Framework/Common.h>

VOLCANO_FRAMEWORK_BEGIN

// Owns the game Scene; game rules remain in the Scene subclass.
class Base {
public:
    enum class State {
        Idle,
        Loading,
        Ready,
        Playing,
        Paused
    };

    using SceneFactory = std::function<std::unique_ptr<World::Scene>()>;

    Base();
    virtual ~Base();
    Base(const Base&) = delete;
    Base& operator=(const Base&) = delete;
    Base(Base&&) = delete;
    Base& operator=(Base&&) = delete;

    void load(SceneFactory factory);
    void synchronizeResources();
    void play();
    void pause();
    void stop();
    void unload();
    void update(Clock::duration elapsed);

    State state() const noexcept;
    World::Scene& scene();
    const World::Scene& scene() const;

protected:
    virtual void onLoadResources(const World::Scene& scene);
    virtual void onUnloadResources() noexcept;
    virtual void onCommitResources() noexcept;
    virtual void onDiscardResources() noexcept;

    virtual void onBeforeUpdate(Clock::duration elapsed);
    virtual void onAfterUpdate(Clock::duration elapsed);

    const Schema::Model& modelResource(const std::filesystem::path& path) const;

private:
    void requireLoaded() const;
    void prepareResources(const World::Scene& scene);

private:
    SceneFactory scene_factory_;
    State state_ = State::Idle;
    std::unique_ptr<World::Scene> scene_;
    std::map<std::filesystem::path, Schema::Model> models_;
    std::map<std::filesystem::path, Schema::Model> pending_models_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_BASE_H
