#include <stdexcept>
#include <utility>

#include <Volcano/ScopeGuard.h>
#include <Volcano/World/Model.h>
#include <Volcano/Framework/Base.h>

#include "Resource.h"

VOLCANO_FRAMEWORK_BEGIN

Base::Base() = default;
Base::~Base() = default;

Base::State Base::state() const noexcept {
    return state_;
}

void Base::requireLoaded() const {
    if (state_ == State::Idle || state_ == State::Loading) {
        throw std::logic_error("Framework operation requires a loaded scene");
    }
}

World::Scene& Base::scene() {
    requireLoaded();
    return *scene_;
}

const World::Scene& Base::scene() const {
    requireLoaded();
    return *scene_;
}

void Base::prepareResources(const World::Scene& candidate) {
    auto rollback = scopeGuard([this] {
        onDiscardResources();
        pending_models_.clear();
    });
    onLoadResources(candidate);
    models_.merge(pending_models_);
    onCommitResources();
    rollback.dismiss();
}

void Base::load(SceneFactory factory) {
    if (state_ != State::Idle) {
        throw std::logic_error("Framework load requires Idle state");
    }
    if (!factory) {
        throw std::invalid_argument("Scene factory must be callable");
    }
    state_ = State::Loading;
    auto rollback = scopeGuard([this] { state_ = State::Idle; });
    auto candidate = factory();
    if (!candidate) {
        throw std::runtime_error("Scene factory returned null");
    }
    prepareResources(*candidate);
    scene_factory_ = std::move(factory);
    scene_ = std::move(candidate);
    state_ = State::Ready;
    rollback.dismiss();
}

void Base::stop() {
    requireLoaded();
    const auto previous = state_;
    state_ = State::Loading;
    auto rollback = scopeGuard([this, previous] { state_ = previous; });
    auto candidate = scene_factory_();
    if (!candidate) {
        throw std::runtime_error("Scene factory returned null");
    }
    prepareResources(*candidate);
    scene_ = std::move(candidate);
    state_ = State::Ready;
    rollback.dismiss();
}

void Base::synchronizeResources() {
    requireLoaded();
    const auto previous = state_;
    state_ = State::Loading;
    auto restore = scopeGuard([this, previous] { state_ = previous; });
    prepareResources(*scene_);
}

void Base::play() {
    if (state_ != State::Ready && state_ != State::Paused) {
        throw std::logic_error("Framework play requires Ready or Paused state");
    }
    state_ = State::Playing;
}

void Base::pause() {
    if (state_ != State::Playing) {
        throw std::logic_error("Framework pause requires Playing state");
    }
    state_ = State::Paused;
}

void Base::unload() {
    if (state_ == State::Loading) {
        throw std::logic_error("Cannot unload while loading resources");
    }
    if (state_ == State::Idle) {
        return;
    }
    state_ = State::Loading;
    onUnloadResources();
    scene_.reset();
    models_.clear();
    scene_factory_ = {};
    state_ = State::Idle;
}

void Base::update(Clock::duration elapsed) {
    if (elapsed < Clock::duration::zero()) {
        throw std::invalid_argument("Framework elapsed time cannot be negative");
    }
    if (state_ == State::Idle) {
        return;
    }
    requireLoaded();
    onBeforeUpdate(elapsed);
    if (state_ == State::Playing) {
        scene_->update(elapsed);
    }
    onAfterUpdate(elapsed);
}

void Base::onLoadResources(const World::Scene& candidate) {
    for (const auto entity : candidate.view<World::Model>()) {
        const auto path = resourcePath(candidate.get<World::Model>(entity).source);
        if (models_.contains(path) || pending_models_.contains(path)) {
            continue;
        }
        auto model = readResource<Schema::Model>(path);
        for (auto& part : model.parts) {
            part.mesh = resourcePath(path.parent_path() / part.mesh);
            part.material = resourcePath(path.parent_path() / part.material);
        }
        pending_models_.emplace(path, std::move(model));
    }
}

const Schema::Model& Base::modelResource(const std::filesystem::path& path) const {
    const auto key = resourcePath(path);
    if (const auto it = pending_models_.find(key); it != pending_models_.end()) {
        return it->second;
    }
    if (const auto it = models_.find(key); it != models_.end()) {
        return it->second;
    }
    throw std::logic_error("Model resource requires synchronizeResources(): " + key.generic_string());
}

void Base::onUnloadResources() noexcept {
}

void Base::onCommitResources() noexcept {
}

void Base::onDiscardResources() noexcept {
}

void Base::onBeforeUpdate(Clock::duration) {
}

void Base::onAfterUpdate(Clock::duration) {
}

VOLCANO_FRAMEWORK_END
