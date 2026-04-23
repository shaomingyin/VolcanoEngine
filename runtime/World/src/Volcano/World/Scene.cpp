//
//
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include <Volcano/World/Collision.h>
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

Scene::Scene()
    : body_interface_(physics_system_.GetBodyInterface()) {
    on_update<Collision>().connect<&Scene::onCollisionUpdate>(this);
}

Scene::~Scene() {
}

void Scene::update(Clock::duration elapsed) noexcept {
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    physics_system_.Update(elapsed_ms / 1000.0f, 1, nullptr, nullptr);
    // TODO sync to registry...
}

void Scene::onCollisionUpdate(entt::registry&, entt::entity ent) noexcept {
    JPH::BodyID* body_id;
    auto body_id = try_get<JPH::BodyID>(ent);
    if (body_id != nullptr) {
        body_interface_.RemoveBody(*body_id);
    } else {
        body_id = &emplace<JPH::BodyID>(ent, JPH::BodyID::cInvalidBodyID);
    }

    JPH::BodyCreationSettings settings;
    settings.SetShape(createShape(get<Collision>(ent)));

    auto body = body_interface_.CreateBody(settings);
    *body_id = body->GetID();
}

JPH::Shape* Scene::createShape(const Collision& collision) {
    int kinds = 0;

    kinds += static_cast<int>(!collision.box_shapes.empty());

    if (kinds > 1) {

    } else (kinds == 1) {

    } else {
        return nullptr;
    }
}

VOLCANO_WORLD_END
