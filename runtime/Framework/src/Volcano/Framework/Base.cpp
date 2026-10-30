#include <chrono>
#include <stdexcept>
#include <thread>
#include <utility>

#include <Volcano/ScopeGuard.h>
#include <Volcano/World/Inherent.h>
#include <Volcano/World/Model.h>
#include <Volcano/Framework/Base.h>
#include <Volcano/Framework/Resources.h>

VOLCANO_FRAMEWORK_BEGIN

Base::Base() = default;
Base::~Base() {
    cancelLoad();
    drainLoad();
    unload();
}

void Base::cancelLoad() noexcept {
    cancellation_.cancel();
}

void Base::checkLoadCancelled() const {
    async::interruption_point(cancellation_);
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
    while (loading_.valid() && !loading_.ready()) {
        processTasks();
        if (!loading_.ready()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

void Base::waitForLoad() {
    VOLCANO_ASSERT(std::this_thread::get_id() == owner_thread_);
    drainLoad();
    if (loading_.valid()) {
        loading_.get();
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

async::task<void> Base::beginLoad(SceneFactory factory) {
    VOLCANO_ASSERT(std::this_thread::get_id() == owner_thread_);
    const auto previous = state_;
    state_ = State::Loading;
    progress_ = {};
    loading_entities_.clear();
    entity_tasks_.clear();
    cancellation_.reset();
    auto restore = scopeGuard([this, previous] {
        state_ = previous;
    });
    const bool replace = static_cast<bool>(factory);
    pending_factory_ = std::move(factory);
    auto prepare = async::spawn(scheduler_, [this, replace] {
        checkLoadCancelled();
        if (replace) {
            pending_scene_ = pending_factory_();
            if (!pending_scene_) {
                throw std::runtime_error("Scene factory returned null");
            }
            checkLoadCancelled();
            pending_physics_ = std::make_unique<Physics::System>(*pending_scene_);
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
        return onLoadScene(candidate);
    });
    auto settled = prepare.then(scheduler_, [this](async::task<void> result) {
        // A derived scene hook can throw after starting its parent entity tasks.
        return async::when_all(entity_tasks_.begin(), entity_tasks_.end())
            .then(scheduler_,
                  [result = std::move(result)](std::vector<async::shared_task<void>> children) mutable {
                      result.get();
                      for (const auto& child : children) {
                          child.get();
                      }
                  });
    });
    auto complete = settled.then(scheduler_, [this, previous, replace](async::task<void> result) {
        try {
            result.get();
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
        } catch (...) {
            onDiscardResources();
            pending_models_.clear();
            pending_physics_.reset();
            pending_scene_.reset();
            loading_entities_.clear();
            entity_tasks_.clear();
            pending_factory_ = {};
            state_ = previous;
            throw;
        }
        loading_entities_.clear();
        entity_tasks_.clear();
        state_ = replace ? State::Ready : previous;
        progress_.percent = 100;
        progress_.completed = progress_.total;
        progress_.content.clear();
    });
    loading_ = complete.share();
    restore.dismiss();
    return loading_.then(async::inline_scheduler(), [] {
    });
}

async::task<void> Base::loadAsync(SceneFactory factory) {
    VOLCANO_ASSERT(state_ == State::Idle);
    VOLCANO_ASSERT(factory);
    return beginLoad(std::move(factory));
}

async::task<void> Base::stopAsync() {
    VOLCANO_ASSERT(state_ != State::Idle && state_ != State::Loading);
    return beginLoad(scene_factory_);
}

async::task<void> Base::synchronizeResourcesAsync() {
    VOLCANO_ASSERT(state_ != State::Idle && state_ != State::Loading);
    return beginLoad({});
}

void Base::load(SceneFactory factory) {
    auto task = loadAsync(std::move(factory));
    drainLoad();
    task.get();
}

void Base::stop() {
    auto task = stopAsync();
    drainLoad();
    task.get();
}

void Base::synchronizeResources() {
    auto task = synchronizeResourcesAsync();
    drainLoad();
    task.get();
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

async::task<void> Base::onLoadScene(const World::Scene& candidate) {
    entity_tasks_.reserve(loading_entities_.size());
    for (const auto& [entity, content] : loading_entities_) {
        // Invoke virtual hooks inside tasks so a synchronous hook failure cannot orphan earlier work.
        auto task = async::spawn(scheduler_, [this, &candidate, entity] {
            checkLoadCancelled();
            return onLoadEntity(candidate, entity);
        });
        auto completed = task.then(scheduler_, [this, entity] {
            checkLoadCancelled();
            loading_entities_.erase(entity);
            ++progress_.completed;
            progress_.percent =
                95.0f * static_cast<float>(progress_.completed) / static_cast<float>(progress_.total);
            progress_.content =
                loading_entities_.empty() ? "Scene resources" : loading_entities_.begin()->second;
        });
        entity_tasks_.push_back(completed.share());
    }
    return async::when_all(entity_tasks_.begin(), entity_tasks_.end())
        .then(scheduler_, [](std::vector<async::shared_task<void>> children) {
            for (auto& child : children) {
                child.get();
            }
        });
}

async::task<void> Base::onLoadEntity(const World::Scene& candidate, entt::entity entity) {
    const auto* model = candidate.try_get<World::Model>(entity);
    if (model == nullptr) {
        return async::make_task();
    }
    const auto path = model->source();
    return async::spawn([this, path] {
        checkLoadCancelled();
        auto resource = Resources::loadModel(path.generic_string());
        checkLoadCancelled();
        return resource;
    }).then(scheduler_, [this, path](std::shared_ptr<Schema::Model> resource) {
        checkLoadCancelled();
        pending_models_.insert_or_assign(path.lexically_normal(), std::move(resource));
    });
}

const Schema::Model& Base::model(const std::filesystem::path& path) const {
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
