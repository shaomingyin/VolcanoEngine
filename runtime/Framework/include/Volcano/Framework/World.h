//
//
#ifndef VOLCANO_FRAMEWORK_WORLD_H
#define VOLCANO_FRAMEWORK_WORLD_H

#include <string>
#include <filesystem>

#include <physfs.h>
#include <nlohmann/json.hpp>

#include <Volcano/Version.h>
#include <Volcano/Scene/Registry.h>
#include <Volcano/Framework/Context.h>

VOLCANO_FRAMEWORK_BEGIN

class World {
public:
    using Clock = Context::Clock;

    enum class State {
        Loading = 0,
        Ready,
        Error
    };

public:
    World(Context& context, const std::filesystem::path& scene_filepath);
    virtual ~World();

public:
    Context& context() noexcept {
        return context_;
    }

    State state() const noexcept {
        return state_;
    }

    const VersionNumber& version() const noexcept {
        return version_;
    }

    const std::string& name() const noexcept {
        return name_;
    }

    bool isPhysicsEnabled() const noexcept {
        return physics_enabled_;
    }

    void enablePhysics() noexcept {
        physics_enabled_ = true;
    }

    void disablePhysics() noexcept {
        physics_enabled_ = false;
    }
    
    unsigned int loadingProgress() const noexcept {
        VOLCANO_ASSERT(state_ == State::Loading);
        return loading_progress_;
    }

    const std::string& loadingMessage() const noexcept {
        VOLCANO_ASSERT(state_ == State::Loading);
        return loading_message_;
    }

    const std::error_code& error() const noexcept {
        VOLCANO_ASSERT(state_ == State::Error);
        return error_;
    }

    virtual void update(Clock::duration elapsed) noexcept;

protected:
    void setLoadingCount(unsigned int count) noexcept {
        VOLCANO_ASSERT(state_ == State::Loading);
        loading_count_ = count;
    }

    void setLoadingProgress(unsigned int progress) noexcept {
        VOLCANO_ASSERT(state_ == State::Loading);
        loading_progress_ = std::clamp(progress, (unsigned int)0, loading_count_);
    }

    void setLoadingMessage(std::string message) noexcept {
        VOLCANO_ASSERT(state_ == State::Loading);
        loading_message_ = std::move(message);
    }

    void setError(std::error_code ec) noexcept {
        state_ = State::Error;
        error_ = ec;
    }

    virtual void loadConfig(const nlohmann::json& config);
    virtual void loadScene(const nlohmann::json& scene);

private:
    Context& context_;
    State state_;
    std::string name_;
    VersionNumber version_;
    Scene::Registry scene_;
    bool physics_enabled_;
    async::task<void> loading_task_;
    unsigned int loading_count_;
    unsigned int loading_progress_;
    std::string loading_message_;
    std::error_code error_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_WORLD_H
