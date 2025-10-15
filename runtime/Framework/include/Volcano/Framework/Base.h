//
//
#ifndef VOLCANO_FRAMEWORK_BASE_H
#define VOLCANO_FRAMEWORK_BASE_H

#include <string>

#include <nlohmann/json.hpp>

#include <Volcano/Framework/Common.h>
#include <Volcano/Framework/Physics.h>
#include <Volcano/Framework/Context.h>

VOLCANO_FRAMEWORK_BEGIN

class Base: public Context {
public:
    Base();
    ~Base() override = default;

public:
    void run();

public:
    const VersionNumber& version() const noexcept override;
    State state() const noexcept override;
    World::Scene& scene() noexcept override;
    void load(const std::string& scene_name) override;
    unsigned int loadingProgress() const override;
    const std::string& loadingMessage() const override;
    const std::error_code& error() const override;
    void quit() noexcept override;
    const std::string& name() const noexcept override;
    unsigned long fps() const noexcept override;
    unsigned long fpsMax() const noexcept override;
    void setFpsMax(unsigned long v) noexcept override;
    bool isPhysicsEnabled() const noexcept override;
    void enablePhysics() noexcept override;
    void disablePhysics() noexcept override;

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
    virtual void frame(Clock::duration elapsed) noexcept;
    virtual void idleFrame(Clock::duration elapsed) noexcept;
    virtual void loadingFrame(Clock::duration elapsed) noexcept;
    virtual void readyFrame(Clock::duration elapsed) noexcept;
    virtual void playingFrame(Clock::duration elapsed) noexcept;
    virtual void pausedFrame(Clock::duration elapsed) noexcept;
    virtual void errorFrame(Clock::duration elapsed) noexcept;

private:
    VersionNumber version_;
    std::string name_;
    State state_;
    Clock::duration min_elapsed_;
    Clock::time_point frame_last_;
    Clock::time_point frame_count_last_;
    unsigned long frame_count_;
    unsigned long frame_count_per_second_;
    World::Scene scene_;
    Physics physics_;
    async::task<void> loading_task_;
    unsigned int loading_count_;
    unsigned int loading_progress_;
    std::string loading_message_;
    std::error_code error_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_BASE_H
