#include <chrono>
#include <stdexcept>
#include <thread>
#include <utility>

#include <stdexec/execution.hpp>
#include <exec/start_detached.hpp>

#include <Volcano/FifoScheduler.h>
#include <Volcano/ScopeGuard.h>
#include <Volcano/World/Inherent.h>
#include <Volcano/World/Model.h>
#include <Volcano/Framework/Base.h>
#include <Volcano/Framework/Resources.h>

VOLCANO_FRAMEWORK_BEGIN

Base::Base() = default;

Base::~Base() {
    cancelLoad();
    try {
        waitForLoad();
    } catch (...) {
    }
    unload();
}

void Base::cancelLoad() noexcept {
    if (cancellation_) {
        cancellation_->request_stop();
    }
}

void Base::checkLoadCancelled() const {
    if (loadCancelled()) {
        throw LoadCanceled{};
    }
}

void Base::setLoadingContent(std::string content) {
    VOLCANO_ASSERT(std::this_thread::get_id() == owner_thread_);
    progress_.content = std::move(content);
}

void Base::processTasks() {
    VOLCANO_ASSERT(std::this_thread::get_id() == owner_thread_);
    onBeforeTasks();
    scheduler_.run_all_tasks();
}

void Base::drainLoad() {
    VOLCANO_ASSERT(std::this_thread::get_id() == owner_thread_);
    while (state_ == State::Loading) {
        processTasks();
        std::this_thread::yield();
    }
}

void Base::waitForLoad() {
    VOLCANO_ASSERT(std::this_thread::get_id() == owner_thread_);
    drainLoad();
    if (last_error_) {
        std::rethrow_exception(std::exchange(last_error_, {}));
    }
}

World::Scene& Base::scene() {
    VOLCANO_ASSERT(state_ != State::Idle && state_ != State::Loading);
    return *scene_;
}

const World::Scene& Base::scene() const {
    VOLCANO_ASSERT(state_ != State::Idle && state_ != State::Loading);
    return *scene_;
}

Physics::System& Base::physics() {
    VOLCANO_ASSERT(state_ != State::Idle && state_ != State::Loading);
    return *physics_;
}

const Physics::System& Base::physics() const {
    VOLCANO_ASSERT(state_ != State::Idle && state_ != State::Loading);
    return *physics_;
}

void Base::requestExit() {
    should_close_ = true;
}

bool Base::shouldClose() const noexcept {
    return should_close_;
}

void Base::beginLoad(SceneFactory factory) {
    VOLCANO_ASSERT(std::this_thread::get_id() == owner_thread_);
    const auto previous = state_;
    state_ = State::Loading;
    progress_ = {};
    loading_entities_.clear();
    cancellation_.emplace();
    last_error_ = {};

    const bool replace = static_cast<bool>(factory);
    pending_factory_ = std::move(factory);

    exec::start_detached(
        stdexec::schedule(scheduler())
        | stdexec::let_value([this, replace, previous] {
            return loadCoroutine(replace, previous);
        })
        | stdexec::upon_error([this](std::exception_ptr error) noexcept {
            last_error_ = std::move(error);
        })
        | stdexec::upon_stopped([]() noexcept {})
    );
}

exec::task<void> Base::loadCoroutine(bool replace, State previous) {
    try {
        checkLoadCancelled();

        if (replace) {
            pending_scene_ = pending_factory_(*this);
            if (!pending_scene_) {
                throw std::runtime_error("Scene factory returned null");
            }
            checkLoadCancelled();
            pending_physics_ = std::make_unique<Physics::System>(pending_scene_->registry());
        }
        checkLoadCancelled();

        const auto& candidate = replace ? *pending_scene_ : *scene_;
        for (const auto [entity] : candidate.storage<entt::entity>()->each()) {
            const auto* inherent = candidate.try_get<World::Inherent>(entity);
            const auto* model = candidate.try_get<World::Model>(entity);
            std::string content;
            if (model != nullptr) {
                content = model->source().generic_string();
            } else if (inherent != nullptr && !inherent->name().empty()) {
                content = inherent->name();
            } else {
                content = "Entity " + std::to_string(entt::to_integral(entity));
            }
            loading_entities_.emplace(entity, std::move(content));
        }
        progress_.total = loading_entities_.size() + 1;
        progress_.content = loading_entities_.empty() ? "Scene resources" : loading_entities_.begin()->second;

        co_await onLoadScene(candidate);

        checkLoadCancelled();
        progress_.percent = 99;
        progress_.content = "Committing resources";
        onCommitResources();
        models_.swap(pending_models_);
        pending_models_.clear();
        if (replace) {
            physics_.reset();
            scene_ = std::move(pending_scene_);
            physics_ = std::move(pending_physics_);
            scene_factory_ = std::move(pending_factory_);
        }

        loading_entities_.clear();
        state_ = replace ? State::Ready : previous;
        progress_.percent = 100;
        progress_.completed = progress_.total;
        progress_.content.clear();
    } catch (...) {
        onDiscardResources();
        pending_models_.clear();
        pending_physics_.reset();
        pending_scene_.reset();
        loading_entities_.clear();
        pending_factory_ = {};
        last_error_ = std::current_exception();
        state_ = previous;
    }
}

void Base::loadAsync(SceneFactory factory) {
    VOLCANO_ASSERT(state_ == State::Idle);
    VOLCANO_ASSERT(factory);
    beginLoad(std::move(factory));
}

void Base::stopAsync() {
    VOLCANO_ASSERT(state_ != State::Idle && state_ != State::Loading);
    beginLoad(scene_factory_);
}

void Base::synchronizeResourcesAsync() {
    VOLCANO_ASSERT(state_ != State::Idle && state_ != State::Loading);
    beginLoad({});
}

void Base::load(SceneFactory factory) {
    loadAsync(std::move(factory));
    waitForLoad();
}

void Base::stop() {
    stopAsync();
    waitForLoad();
}

void Base::synchronizeResources() {
    synchronizeResourcesAsync();
    waitForLoad();
}

void Base::play() {
    VOLCANO_ASSERT(state_ == State::Ready || state_ == State::Paused);
    state_ = State::Playing;
}

void Base::pause() {
    VOLCANO_ASSERT(state_ == State::Playing);
    state_ = State::Paused;
}

void Base::unload() {
    VOLCANO_ASSERT(state_ != State::Loading);
    if (state_ == State::Idle) {
        return;
    }
    state_ = State::Loading;
    onUnloadResources();
    pending_models_.clear();
    models_.clear();
    physics_.reset();
    scene_.reset();
    scene_factory_ = {};
    state_ = State::Idle;
}

void Base::update(Clock::duration elapsed) {
    VOLCANO_ASSERT(elapsed >= Clock::duration::zero());
    processTasks();
    if (last_error_) {
        std::rethrow_exception(std::exchange(last_error_, {}));
    }
    if (state_ == State::Idle || state_ == State::Loading) {
        return;
    }
    onBeforeUpdate(elapsed);
    if (state_ == State::Playing) {
        scene_->update(elapsed);
        physics_->update(elapsed);
    }
    onAfterUpdate(elapsed);
}

exec::task<void> Base::onLoadScene(const World::Scene& candidate) {
    while (!loading_entities_.empty()) {
        auto it = loading_entities_.begin();
        const auto entity = it->first;
        checkLoadCancelled();
        co_await onLoadEntity(candidate, entity);
        checkLoadCancelled();
        loading_entities_.erase(entity);
        ++progress_.completed;
        progress_.percent =
            95.0f * static_cast<float>(progress_.completed) / static_cast<float>(progress_.total);
        progress_.content =
            loading_entities_.empty() ? "Scene resources" : loading_entities_.begin()->second;
    }
}

exec::task<void> Base::onLoadEntity(const World::Scene& candidate, entt::entity entity) {
    const auto* model = candidate.try_get<World::Model>(entity);
    if (model == nullptr) {
        co_return;
    }
    const auto path = model->source();
    checkLoadCancelled();
    co_await exec::reschedule_coroutine_on(stdexec::get_parallel_scheduler());
    checkLoadCancelled();
    auto resource = Resources::loadModel(path.generic_string());
    checkLoadCancelled();
    co_await exec::reschedule_coroutine_on(scheduler());
    checkLoadCancelled();
    pending_models_.insert_or_assign(path.lexically_normal(), std::move(resource));
}

const Asset::Model& Base::model(const std::filesystem::path& path) const {
    const auto key = path.lexically_normal();
    if (const auto found = pending_models_.find(key); found != pending_models_.end()) {
        return *found->second;
    }
    const auto found = models_.find(key);
    VOLCANO_ASSERT(found != models_.end());
    return *found->second;
}

void Base::onBeforeTasks() {}

void Base::onUnloadResources() noexcept {}

void Base::onCommitResources() noexcept {}

void Base::onDiscardResources() noexcept {}

void Base::onBeforeUpdate(Clock::duration) {}

void Base::onAfterUpdate(Clock::duration) {}

VOLCANO_FRAMEWORK_END
