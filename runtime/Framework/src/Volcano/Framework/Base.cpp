//
//
#include <thread>
#include <stdexcept>
#include <filesystem>

#include <physfs.h>

#include <Volcano/Error.h>
#include <Volcano/Framework/JsonUtils.h>
#include <Volcano/Framework/Base.h>

VOLCANO_FRAMEWORK_BEGIN

using namespace std::chrono_literals;

Base::Base()
    : state_(State::Idle)
    , frame_last_(Clock::now())
    , frame_count_last_(frame_last_)
    , frame_count_(0)
    , frame_count_per_second_(0)
    , physics_(scene_) {
}

void Base::run() {
    VOLCANO_ASSERT(state_ == State::Idle);

    state_ = State::Loading;
    loading_task_ = async::spawn(async::thread_scheduler(), [this] {
        auto config = JsonUtils::loadFromPhysFile("/Config.json");
        auto perf_dirpath = PHYSFS_getPrefDir(appOrganization().c_str(), appName().c_str());
        auto user_config_filepath = std::filesystem::path(perf_dirpath) / "VolcanoConfig.h";
        auto user_config = JsonUtils::loadFromFile(user_config_filepath.c_str());
        config.merge_patch(user_config);
        loadConfig(config);
        auto startup_scene = JsonUtils::loadFromPhysFile("/World/Startup.json");
        loadScene(startup_scene);
    });

    while (state_ != State::Idle) {
        auto now = Clock::now();
        if ((now - frame_count_last_) >= 1s) {
            frame_count_per_second_ = frame_count_;
            frame_count_ = 0;
            frame_count_last_ = now;
        }
        frame(now - frame_last_);
        auto update_elapsed = Clock::now() - now;
        if (update_elapsed < min_elapsed_) {
            std::this_thread::sleep_for(min_elapsed_ - update_elapsed);
        }
    }
}

const VersionNumber& Base::version() const noexcept {
    return version_;
}

Base::State Base::state() const noexcept {
    return state_;
}

World::Scene& Base::scene() noexcept {
    return scene_;
}

void Base::load(const std::string& scene_name) {
    state_ = State::Loading;
    loading_task_ = async::spawn(async::thread_scheduler(), [this, scene_name] {
        auto scene_filepath = std::format("/World/{}.json", scene_name);
        auto scene = JsonUtils::loadFromPhysFile(scene_filepath.c_str());
        loadScene(scene);
    });
}

unsigned int Base::loadingProgress() const {
    VOLCANO_ASSERT(state_ == State::Loading);
    return loading_progress_;
}

const std::string& Base::loadingMessage() const {
    VOLCANO_ASSERT(state_ == State::Loading);
    return loading_message_;
}

const std::error_code& Base::error() const {
    VOLCANO_ASSERT(state_ == State::Error);
    return error_;
}

void Base::quit() noexcept {
    state_ = State::Idle;
}

const std::string& Base::name() const noexcept {
    return name_;
}

unsigned long Base::fps() const noexcept {
    return frame_count_per_second_;
}

unsigned long Base::fpsMax() const noexcept {
    return (1000000000ns / min_elapsed_);
}

void Base::setFpsMax(unsigned long v) noexcept {
    if (v < 1) {
        v = 1;
    }
    min_elapsed_ = (1000000000ns / v);
}

bool Base::isPhysicsEnabled() const noexcept {
    return physics_.isEnabled();
}

void Base::enablePhysics() noexcept {
    physics_.enable();
}

void Base::disablePhysics() noexcept {
    physics_.disable();
}

void Base::loadConfig(const nlohmann::json& config) {
    version_ = JsonUtils::versionValue(config, "/version", { 0, 0, 0 });
    name_ = JsonUtils::value<std::string>(config, "/name", "Untitled");

    int fps_max = JsonUtils::value<int>(config, "/fpsMax", 60);
    setFpsMax(fps_max);
}

void Base::loadScene(const nlohmann::json& scene) {
}

void Base::frame(Clock::duration elapsed) noexcept {
    runAllTasks();
    switch (state_) {
    case State::Playing:
        playingFrame(elapsed);
        break;
    case State::Paused:
        pausedFrame(elapsed);
        break;
    case State::Loading:
        loadingFrame(elapsed);
        break;
    case State::Ready:
        readyFrame(elapsed);
        break;
    case State::Idle:
        idleFrame(elapsed);
        break;
    case State::Error:
        errorFrame(elapsed);
        break;
    default:
        break;
    }
}

void Base::idleFrame(Clock::duration elapsed) noexcept {
}

void Base::loadingFrame(Clock::duration elapsed) noexcept {
    if (loading_task_.ready()) {
        try {
            loading_task_.get();
            state_ = State::Ready;
        } catch (const std::bad_alloc& e) {
            setError(std::make_error_code(std::errc::not_enough_memory));
        } catch (const std::invalid_argument& e) {
            setError(std::make_error_code(std::errc::invalid_argument));
        } catch (const std::out_of_range& e) {
            setError(std::make_error_code(std::errc::result_out_of_range));
        } catch (const std::exception& e) {
            setError(UnknownError());
        }
    }
}

void Base::readyFrame(Clock::duration elapsed) noexcept {
}

void Base::playingFrame(Clock::duration elapsed) noexcept {
    physics_.frame(elapsed);
}

void Base::pausedFrame(Clock::duration elapsed) noexcept {
}

void Base::errorFrame(Clock::duration elapsed) noexcept {
}

VOLCANO_FRAMEWORK_END
