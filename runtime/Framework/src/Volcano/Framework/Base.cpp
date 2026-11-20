#include <chrono>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

#include <async++.h>

#include <Volcano/ScopeGuard.h>
#include <Volcano/Transform.h>
#include <Volcano/World/DirectionalLight.h>
#include <Volcano/World/Inherent.h>
#include <Volcano/World/Model.h>
#include <Volcano/World/OrthographicCamera.h>
#include <Volcano/World/PerspectiveCamera.h>
#include <Volcano/World/PointLight.h>
#include <Volcano/World/RigidBody.h>
#include <Volcano/World/SpotLight.h>
#include <Volcano/World/Texture.h>
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
    loading_.cancel();
}

void Base::schedule(async::task_run_handle task) {
    scheduler_.schedule(std::move(task));
}

void Base::processTasks() {
    VOLCANO_ASSERT(std::this_thread::get_id() == owner_thread_);
    onBeforeTasks();
    scheduler_.run_all_tasks();
}

void Base::waitForLoad() {
    VOLCANO_ASSERT(std::this_thread::get_id() == owner_thread_);
    while (state_ == State::Loading) {
        processTasks();
        std::this_thread::yield();
    }
    loading_.rethrowIfError();
}

World::Scene& Base::scene() {
    if (state_ == State::Idle || state_ == State::Loading) {
        throw std::logic_error("Framework scene is unavailable in the current state");
    }
    return *scene_;
}

const World::Scene& Base::scene() const {
    if (state_ == State::Idle || state_ == State::Loading) {
        throw std::logic_error("Framework scene is unavailable in the current state");
    }
    return *scene_;
}

Physics::System& Base::physics() {
    if (state_ == State::Idle || state_ == State::Loading) {
        throw std::logic_error("Framework physics is unavailable in the current state");
    }
    return *physics_;
}

const Physics::System& Base::physics() const {
    if (state_ == State::Idle || state_ == State::Loading) {
        throw std::logic_error("Framework physics is unavailable in the current state");
    }
    return *physics_;
}

void Base::requestExit() {
    should_close_ = true;
}

bool Base::shouldClose() const noexcept {
    return should_close_;
}

void Base::load(std::filesystem::path scenePath, SceneFactory factory) {
    if (state_ != State::Idle) {
        throw std::logic_error("Framework load requires Idle state");
    }
    VOLCANO_ASSERT(factory);
    beginLoad(std::move(scenePath), std::move(factory));
}

void Base::synchronizeResources() {
    if (state_ == State::Idle || state_ == State::Loading) {
        throw std::logic_error("Framework synchronizeResources requires a loaded state");
    }
    beginLoad({}, {});
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
        throw std::logic_error("Framework cannot unload while Loading");
    }
    if (state_ == State::Idle) {
        return;
    }
    state_ = State::Loading;
    onUnloadResources();
    loading_.discard();
    models_.clear();
    physics_.reset();
    scene_.reset();
    state_ = State::Idle;
}

void Base::update(Clock::duration elapsed) {
    VOLCANO_ASSERT(elapsed >= Clock::duration::zero());
    processTasks();
    loading_.rethrowIfError();
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
    std::vector<entt::entity> entities;
    entities.reserve(loading_.entities.size());
    for (const auto& [entity, name] : loading_.entities) {
        entities.push_back(entity);
    }
    std::vector<async::task<void>> tasks;
    tasks.reserve(entities.size());
    for (const auto entity : entities) {
        loading_.checkCancelled();
        tasks.push_back(onLoadEntity(candidate, entity).then(*this, [this, entity]() {
            loading_.completeEntity(entity);
        }));
    }
    return async::when_all(tasks).then(*this, [this](std::vector<async::task<void>> results) {
        for (auto& result : results) {
            result.get();
        }
        loading_.checkCancelled();
    });
}

async::task<void> Base::onLoadEntity(const World::Scene& candidate, entt::entity entity) {
    const auto* model = candidate.try_get<World::Model>(entity);
    if (model == nullptr) {
        return async::make_task();
    }
    const auto path = model->source();
    loading_.checkCancelled();
    return async::spawn([this, path]() {
        loading_.checkCancelled();
        return Resources::loadModel(path.generic_string());
    }).then(*this, [this, path](std::shared_ptr<Asset::Model> resource) {
        loading_.checkCancelled();
        loading_.models.insert_or_assign(path.lexically_normal(), std::move(resource));
    });
}

const Asset::Model& Base::model(const std::filesystem::path& path) const {
    const auto key = path.lexically_normal();
    if (const auto found = loading_.models.find(key); found != loading_.models.end()) {
        return *found->second;
    }
    const auto found = models_.find(key);
    VOLCANO_ASSERT(found != models_.end());
    return *found->second;
}

World::Scene* Base::currentScene() noexcept {
    return loading_.scene ? loading_.scene.get() : scene_.get();
}

void Base::onBeforeTasks() {}

void Base::onUnloadResources() noexcept {}

void Base::onCommitResources() noexcept {}

void Base::onDiscardResources() noexcept {}

void Base::onBeforeUpdate(Clock::duration) {}

void Base::onAfterUpdate(Clock::duration) {}

void Base::beginLoad(std::filesystem::path scenePath, SceneFactory factory) {
    VOLCANO_ASSERT(std::this_thread::get_id() == owner_thread_);
    const auto previous = state_;
    state_ = State::Loading;
    loading_.reset();

    const bool replace = static_cast<bool>(factory);
    loading_.factory = std::move(factory);

    async::spawn(*this, [this, replace, scenePath = std::move(scenePath)]() {
        loading_.checkCancelled();

        if (!replace) {
            loading_.collectEntities(*scene_);
            return onLoadScene(*scene_);
        }

        loading_.scene = loading_.factory(*this);
        if (!loading_.scene) {
            throw std::runtime_error("Scene factory returned null");
        }
        loading_.checkCancelled();

        loading_.setContent(scenePath.generic_string());
        return async::spawn([this, scenePath]() {
            loading_.checkCancelled();
            return Resources::loadScene(scenePath.generic_string());
        }).then(*this, [this](std::shared_ptr<Asset::Scene> sceneAsset) {
            loading_.checkCancelled();
            LoadingState::populateScene(*loading_.scene, *sceneAsset);
            loading_.physics = std::make_unique<Physics::System>(*loading_.scene);
            if (sceneAsset->has_gravity()) {
                const auto& g = sceneAsset->gravity();
                loading_.physics->setGravity({g.x(), g.y(), g.z()});
            }
            loading_.collectEntities(*loading_.scene);
            return onLoadScene(*loading_.scene);
        });
    }).then(*this, [this, replace, previous](async::task<void> loadTask) {
        try {
            loadTask.get();
            loading_.checkCancelled();
            loading_.beginCommit();
            onCommitResources();
            models_.swap(loading_.models);
            loading_.models.clear();
            if (replace) {
                physics_.reset();
                scene_ = std::move(loading_.scene);
                physics_ = std::move(loading_.physics);
            }

            state_ = replace ? State::Ready : previous;
            loading_.completeCommit();
        } catch (...) {
            onDiscardResources();
            loading_.discard();
            loading_.error = std::current_exception();
            state_ = previous;
        }
    });
}

void Base::LoadingState::cancel() noexcept {
    cancellation.cancel();
}

bool Base::LoadingState::cancelled() const noexcept {
    return cancellation.is_canceled();
}

void Base::LoadingState::checkCancelled() const {
    async::interruption_point(cancellation);
}

void Base::LoadingState::rethrowIfError() {
    if (error) {
        std::rethrow_exception(std::exchange(error, {}));
    }
}

void Base::LoadingState::setContent(std::string content) {
    progress.content = std::move(content);
}

void Base::LoadingState::collectEntities(const World::Scene& candidate) {
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
        entities.emplace(entity, std::move(content));
    }
    progress.total = entities.size() + 1;
    progress.content = entities.empty() ? "Scene resources" : entities.begin()->second;
}

void Base::LoadingState::completeEntity(entt::entity entity) {
    entities.erase(entity);
    ++progress.completed;
    progress.percent =
        95.0f * static_cast<float>(progress.completed) / static_cast<float>(progress.total);
    progress.content = entities.empty() ? "Scene resources" : entities.begin()->second;
}

void Base::LoadingState::beginCommit() {
    progress.percent = 99;
    progress.content = "Committing resources";
}

void Base::LoadingState::completeCommit() noexcept {
    entities.clear();
    progress.percent = 100;
    progress.completed = progress.total;
    progress.content.clear();
}

void Base::LoadingState::discard() noexcept {
    models.clear();
    physics.reset();
    scene.reset();
    entities.clear();
    factory = nullptr;
}

void Base::LoadingState::reset() noexcept {
    cancellation.reset();
    error = nullptr;
    progress = {};
    discard();
}

void Base::LoadingState::populateScene(World::Scene& target, const Asset::Scene& source) {
    if (!source.name().empty()) {
        target.setName(source.name());
    }
    if (!source.description().empty()) {
        target.setDescription(source.description());
    }
    for (const auto& entityDesc : source.entities()) {
        const auto entity = target.create();
        if (!entityDesc.name().empty() || entityDesc.has_enabled() || entityDesc.has_visible()) {
            target.emplace<World::Inherent>(
                entity,
                entityDesc.name(),
                entityDesc.has_enabled() ? entityDesc.enabled() : true,
                entityDesc.has_visible() ? entityDesc.visible() : true);
        }
        if (entityDesc.has_transform()) {
            const auto& src = entityDesc.transform();
            auto& transform = target.emplace<Transform>(entity);
            if (src.has_translation()) {
                transform.setTranslation({src.translation().x(), src.translation().y(), src.translation().z()});
            }
            if (src.has_scale()) {
                transform.setScale({src.scale().x(), src.scale().y(), src.scale().z()});
            }
            if (src.has_rotation()) {
                const Eigen::Quaternionf rotation{
                    src.rotation().w(), src.rotation().x(), src.rotation().y(), src.rotation().z()};
                if (rotation.squaredNorm() > 0.0f) {
                    transform.setRotation(rotation.normalized());
                }
            }
        }
        if (entityDesc.has_model()) {
            const auto& src = entityDesc.model();
            auto& model = target.emplace<World::Model>(entity, std::filesystem::path(src.source()));
            for (const auto& mat : src.materials()) {
                auto& dst = model.material(mat.part());
                if (!mat.albedo_texture().empty()) {
                    dst.setTexture(World::Material::Slot::Albedo,
                                   std::make_unique<World::Texture>(std::filesystem::path(mat.albedo_texture())));
                }
            }
        }
        if (entityDesc.has_perspective_camera()) {
            const auto& src = entityDesc.perspective_camera();
            auto& cam = target.emplace<World::PerspectiveCamera>(entity);
            if (src.has_fov()) {
                cam.setFov(src.fov());
            }
            if (src.has_aspect()) {
                cam.setAspect(src.aspect());
            }
            if (src.has_znear() || src.has_zfar()) {
                cam.setZRange(src.has_znear() ? src.znear() : cam.zRange().x(),
                              src.has_zfar() ? src.zfar() : cam.zRange().y());
            }
            if (!src.hud_texture().empty()) {
                cam.setHud(std::make_unique<World::Texture>(std::filesystem::path(src.hud_texture())));
            }
        }
        if (entityDesc.has_orthographic_camera()) {
            const auto& src = entityDesc.orthographic_camera();
            auto& cam = target.emplace<World::OrthographicCamera>(entity);
            if (src.has_left() || src.has_right() || src.has_bottom() || src.has_top()) {
                cam.setRect(src.has_left() ? src.left() : cam.rect().x(),
                            src.has_right() ? src.right() : cam.rect().y(),
                            src.has_bottom() ? src.bottom() : cam.rect().z(),
                            src.has_top() ? src.top() : cam.rect().w());
            }
            if (src.has_znear() || src.has_zfar()) {
                cam.setZRange(src.has_znear() ? src.znear() : cam.zRange().x(),
                              src.has_zfar() ? src.zfar() : cam.zRange().y());
            }
            if (!src.hud_texture().empty()) {
                cam.setHud(std::make_unique<World::Texture>(std::filesystem::path(src.hud_texture())));
            }
        }
        if (entityDesc.has_directional_light()) {
            const auto& src = entityDesc.directional_light();
            auto& light = target.emplace<World::DirectionalLight>(entity);
            if (src.has_direction()) {
                light.setDirection({src.direction().x(), src.direction().y(), src.direction().z()});
            }
            if (src.has_color()) {
                light.setColor({src.color().x(), src.color().y(), src.color().z()});
            }
            if (src.has_strength()) {
                light.setStrength(src.strength());
            }
        }
        if (entityDesc.has_point_light()) {
            const auto& src = entityDesc.point_light();
            auto& light = target.emplace<World::PointLight>(entity);
            if (src.has_color()) {
                light.setColor({src.color().x(), src.color().y(), src.color().z()});
            }
            if (src.has_strength()) {
                light.setStrength(src.strength());
            }
            if (src.has_range()) {
                light.setRange(src.range());
            }
        }
        if (entityDesc.has_spot_light()) {
            const auto& src = entityDesc.spot_light();
            auto& light = target.emplace<World::SpotLight>(entity);
            if (src.has_direction()) {
                light.setDirection({src.direction().x(), src.direction().y(), src.direction().z()});
            }
            if (src.has_color()) {
                light.setColor({src.color().x(), src.color().y(), src.color().z()});
            }
            if (src.has_strength()) {
                light.setStrength(src.strength());
            }
            if (src.has_range()) {
                light.setRange(src.range());
            }
            if (src.has_inner_angle() || src.has_outer_angle()) {
                light.setAngles(src.has_inner_angle() ? src.inner_angle() : light.innerAngle(),
                                src.has_outer_angle() ? src.outer_angle() : light.outerAngle());
            }
        }
        if (entityDesc.has_rigid_body()) {
            const auto& src = entityDesc.rigid_body();
            auto& body = target.emplace<World::RigidBody>(entity);
            if (src.has_box()) {
                World::RigidBody::Box box;
                if (src.box().has_half_extents()) {
                    const auto& ext = src.box().half_extents();
                    box.setHalfExtents({ext.x(), ext.y(), ext.z()});
                }
                body.setShape(box);
            } else if (src.has_sphere()) {
                World::RigidBody::Sphere sphere;
                if (src.sphere().has_radius()) {
                    sphere.setRadius(src.sphere().radius());
                }
                body.setShape(sphere);
            } else if (src.has_capsule()) {
                World::RigidBody::Capsule capsule;
                if (src.capsule().has_half_height() || src.capsule().has_radius()) {
                    capsule.setDimensions(
                        src.capsule().has_half_height() ? src.capsule().half_height() : capsule.halfHeight(),
                        src.capsule().has_radius() ? src.capsule().radius() : capsule.radius());
                }
                body.setShape(capsule);
            }
            if (src.has_offset()) {
                Transform offset;
                if (src.offset().has_translation()) {
                    const auto& t = src.offset().translation();
                    offset.setTranslation({t.x(), t.y(), t.z()});
                }
                if (src.offset().has_scale()) {
                    const auto& s = src.offset().scale();
                    offset.setScale({s.x(), s.y(), s.z()});
                }
                if (src.offset().has_rotation()) {
                    const Eigen::Quaternionf q{
                        src.offset().rotation().w(),
                        src.offset().rotation().x(),
                        src.offset().rotation().y(),
                        src.offset().rotation().z()};
                    if (q.squaredNorm() > 0.0f) {
                        offset.setRotation(q.normalized());
                    }
                }
                body.setOffset(offset);
            }
            if (src.has_motion()) {
                switch (src.motion()) {
                case Asset::MOTION_STATIC:
                    body.setMotion(World::RigidBody::Motion::Static);
                    break;
                case Asset::MOTION_KINEMATIC:
                    body.setMotion(World::RigidBody::Motion::Kinematic);
                    break;
                case Asset::MOTION_DYNAMIC:
                default:
                    body.setMotion(World::RigidBody::Motion::Dynamic);
                    break;
                }
            }
            if (src.has_mass()) {
                body.setMass(src.mass());
            }
            if (src.has_friction()) {
                body.setFriction(src.friction());
            }
            if (src.has_restitution()) {
                body.setRestitution(src.restitution());
            }
            if (src.has_linear_damping()) {
                body.setLinearDamping(src.linear_damping());
            }
            if (src.has_angular_damping()) {
                body.setAngularDamping(src.angular_damping());
            }
            if (src.has_gravity_factor()) {
                body.setGravityFactor(src.gravity_factor());
            }
        }
    }
}

VOLCANO_FRAMEWORK_END
