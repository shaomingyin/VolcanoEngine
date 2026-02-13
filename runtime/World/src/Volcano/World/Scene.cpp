//
//
#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>

#include <Volcano/World/Transform.h>
#include <Volcano/World/Kinetics.h>
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

using BodySet = std::set<JPH::Body*>;

Scene::Scene()
	: global_(*this, create()) {
	on_construct<Transform>().connect<&Scene::onTransformAdded>(this);
	on_destroy<Transform>().connect<&Scene::onTransformRemoved>(this);
	on_update<Transform>().connect<&Scene::onTransformUpdated>(this);

	on_construct<Kinetics>().connect<&Scene::onKineticsAdded>(this);
	on_destroy<Kinetics>().connect<&Scene::onKineticsRemoved>(this);
	on_update<Kinetics>().connect<&Scene::onKineticsUpdated>(this);
}

Scene::~Scene() {
	on_construct<Transform>().disconnect<&Scene::onTransformAdded>(this);
	on_destroy<Transform>().disconnect<&Scene::onTransformRemoved>(this);
	on_update<Transform>().disconnect<&Scene::onTransformUpdated>(this);

	on_construct<Kinetics>().disconnect<&Scene::onKineticsAdded>(this);
	on_destroy<Kinetics>().disconnect<&Scene::onKineticsRemoved>(this);
	on_update<Kinetics>().disconnect<&Scene::onKineticsUpdated>(this);
}

void Scene::update(Clock::duration elapsed) {
	auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
	//physics_system_.Update(float(elapsed_ms) / 1000.0f, 1, );

}

void Scene::onTransformAdded(entt::entity ent) {
	if (try_get<Kinetics>(ent) != nullptr) {
		onKineticsAdded(ent);
	}
}

void Scene::onTransformRemoved(entt::entity ent) {
	if (try_get<Kinetics>(ent) != nullptr) {
		remove<Kinetics>(ent);
	}
}

void Scene::onTransformUpdated(entt::entity ent) {
	if (try_get<Kinetics>(ent) != nullptr) {
		patch<Kinetics>(ent, [](auto& kinetics) {
			// TODO
		});
	}
}

void Scene::onKineticsAdded(entt::entity ent) {
	auto transform = try_get<Transform>(ent);
	if (transform == nullptr) {
		return;
	}

	BodySet body_set;
	auto& kinetics = get<Kinetics>(ent);
	auto& body_interface = physics_system_.GetBodyInterface();

	for (auto& shape: kinetics.shape_set) {
		auto t = (*transform) * shape.offset;

		JPH::BodyCreationSettings settings;
		if (auto p = std::get_if<Kinetics::Box>(&shape.def)) {
			auto v = p->half_size * t.scale;
			settings.SetShape(new JPH::BoxShape(JPH::Vec3Arg{ v.x(), v.y(), v.z() }));
		} else if (auto p = std::get_if<Kinetics::Sphere>(&shape.def)) {
			auto v = p->radius * t.scale.x(); // FIXME
			settings.SetShape(new JPH::SphereShape(v));
		} else {
			continue;
		}

		auto body = body_interface.CreateBody(settings);
		if (body == nullptr) {
			continue;
		}

		body->SetUserData(reinterpret_cast<JPH::uint64>(&shape));

		auto id = body->GetID();
		body_interface.SetPosition(id, { t.translation.x(), t.translation.y(), t.translation.z() }, JPH::EActivation::DontActivate);
		body_interface.SetRotation(id, { t.rotation.x(), t.rotation.y(), t.rotation.z(), t.rotation.w()}, JPH::EActivation::DontActivate);
		body_interface.AddBody(id, JPH::EActivation::Activate);
		body_set.insert(body);
	}

	emplace<BodySet>(ent, std::move(body_set));
}

void Scene::onKineticsRemoved(entt::entity ent) {
	auto& body_set = get<BodySet>(ent);
	auto& body_interface = physics_system_.GetBodyInterface();
	for (auto body: body_set) {
		body_interface.RemoveBody(body->GetID());
		body_interface.DestroyBody(body->GetID());
	}
	remove<BodySet>(ent);
}

void Scene::onKineticsUpdated(entt::entity ent) {
	auto transform = try_get<Transform>(ent);
	if (transform == nullptr) {
		return;
	}

	auto& kinetics = get<Kinetics>(ent);
	auto& body_set = get<BodySet>(ent);
	auto& body_interface = physics_system_.GetBodyInterface();

	for (auto body: body_set) {
		auto shape = reinterpret_cast<Kinetics::Shape*>(body->GetUserData());
		auto t = (*transform) * shape->offset;

		if (auto def = std::get_if<Kinetics::Box>(&shape->def)) {
			auto v = def->half_size * t.scale;
			// TODO
		} else if (auto def = std::get_if<Kinetics::Sphere>(&shape->def)) {
			auto v = def->radius * t.scale.x(); // FIXME
			// TODO
		} else {
			continue;
		}

		auto id = body->GetID();
		body_interface.SetPosition(id, { t.translation.x(), t.translation.y(), t.translation.z() }, JPH::EActivation::DontActivate);
		body_interface.SetRotation(id, { t.rotation.x(), t.rotation.y(), t.rotation.z(), t.rotation.w() }, JPH::EActivation::DontActivate);
	}
}

VOLCANO_WORLD_END
