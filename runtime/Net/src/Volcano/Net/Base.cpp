//
//
#include <vector>

#include <Volcano/ScopeGuard.h>
#include <Volcano/World/BoxRigidBody.h>
#include <Volcano/World/CapsuleRigidBody.h>
#include <Volcano/World/ConeRigidBody.h>
#include <Volcano/World/CylinderRigidBody.h>
#include <Volcano/World/SphereRigidBody.h>
#include <Volcano/World/StaticPlaneRigidBody.h>
#include <Volcano/World/TriangleMeshRigidBody.h>
#include <Volcano/Net/Base.h>

VOLCANO_NET_BEGIN

Base::Base()
	: state_(State::Idle)
	, loading_progress_(0) {
}

void Base::run() {
	if (state_ != State::Idle) {
		throw std::runtime_error("Run in invalid state.");
	}

	state_ = State::Loading;

	nlohmann::json config_json;
	try { config_json = nlohmann::parseFromPhysFS("/Config.json"); } catch (...) {}
	loadConfig(config_json);

	loading_task_ = async::spawn(async::thread_scheduler(), std::bind(&Base::load, this));

	auto simulate = [this] {
		ticker_.reset();
		while (state_ != State::Idle) {
			if (ticker_.step()) {
				tick(ticker_.elapsed());
			}
		}
	};

	framer_.reset();

	if (!threaded_simulation_) {
		simulate();
		return;
	}

	auto simulation_thread = std::thread(simulate);
	auto simulation_thread_guard = scopeGuard([&simulation_thread] {
		simulation_thread.join();
	});

	while (state_ != State::Idle) {
		if (framer_.step()) {
			frame(framer_.elapsed());
		}
	}
}

void Base::clear() {
	name_.clear();
	scene_.clear();
	loading_message_.clear();
	loading_progress_ = 0;
	error_message_.clear();
	state_ = State::Idle;
}

void Base::tick(Duration elapsed) {
	if (!threaded_simulation_) {
		task_scheduler_.run_all_tasks();
	}

	switch (state_) {
	case State::Playing:
		scene_.update(elapsed);
		break;
	case State::Loading:
		if (!threaded_simulation_ && loading_task_.ready()) {
			try {
				loading_task_.get();
				state_ = State::Ready;
			} catch (std::exception& ex) {
				error(ex.what());
			}
		}
		break;
	default:
		break;
	}

	if (!threaded_simulation_) {
		frame(elapsed);
	}
}

void Base::frame(Duration elapsed) {
	if (threaded_simulation_) {
		task_scheduler_.run_all_tasks();
	}
}

void Base::loadConfig(const nlohmann::json& json) {
	threaded_simulation_ = nlohmann::parseBool(json, "threadedSimulation", false);
	logInfo("threadedSimulation: {}", threaded_simulation_);

	ticker_.setMax(nlohmann::parseInt(json, "tpsMax", 60));
	logInfo("tpsMax: {}", ticker_.max());

	framer_.setMax(nlohmann::parseInt(json, "fpsMax", 60));
	logInfo("fpsMax: {}", framer_.max());
}

void Base::loadScene(const nlohmann::json& json) {
	loading_message_ = "Loading scene...";

	if (!json.is_object()) {
		throw std::runtime_error("Scene is not an object.");
	}

	name_ = nlohmann::parseString(json, "name", "Unnamed");

	auto map_it = json.find("map");
	if (map_it != json.end() && map_it->is_object()) {
		loadMap(*map_it, scene_.global());
	}

	if (nlohmann::parseBool(json, "physicsEnabled")) {
		logInfo("Physics enabled.");
		scene_.enablePhysics();
	} else {
		logInfo("Physics disabled.");
		scene_.disablePhysics();
	}

	auto gravity = nlohmann::parseVector3f(json, "gravity", Eigen::Vector3f(0.0f, -9.8f, 0.0f));
	scene_.setGravity(gravity);

	auto entities_it = json.find("entities");
	if (entities_it != json.end()) {
		loading_message_ = "Loading entities...";
		if (!entities_it->is_array()) {
			throw std::runtime_error(std::format("Node 'entities' in scene {} is not an array.", name_));
		}
		for (const nlohmann::json& entity: *entities_it) {
			loadingCancelPoint();
			loadEntity(entity, scene_.createEntity());
		}
	}
}

void Base::loadMap(const nlohmann::json& json, entt::handle map) {
}

void Base::loadEntity(const nlohmann::json& json, entt::handle entity) {
	auto& inherency = entity.emplace<World::Inherency>(nlohmann::parseString(json, "name"));

	if (nlohmann::parseBool(json, "enabled", true)) {
		inherency.enable();
	} else {
		inherency.disable();
	}

	inherency.transform() = nlohmann::parseAffine3f(json, "transform");

	auto rigid_bodys_it = json.find("rigidBodys");
	if (rigid_bodys_it != json.end() && rigid_bodys_it->is_array()) {
		for(const nlohmann::json rigid_body: *rigid_bodys_it) {
			loadRigidBody(rigid_body, entity);
		}
	}
}

void Base::load() {
	VOLCANO_ASSERT(state_ == State::Loading);

	name_.clear();
	scene_.clear();
	loading_message_.clear();
	loading_progress_ = 0;
	loading_cancellation_.reset();

	auto manifest = nlohmann::parseFromPhysFS("/Manifest.json");
	if (!manifest.is_object()) {
		throw std::runtime_error("Manifest is not an object.");
	}

	loadScene(nlohmann::parseString(manifest, "firstScene", "/Startup.json"));
}

void Base::loadRigidBody(const nlohmann::json& json, entt::handle entity) {
	if (!json.is_object()) {
		throw std::runtime_error("Invalid rigid body node type.");
	}

	auto type_it = json.find("type");
	if (type_it == json.end() || !type_it->is_string()) {
		throw std::runtime_error("Rigid body node has no valid 'type' field.");
	}

	auto mass_it = json.find("mass");
	if (mass_it == json.end() || !mass_it->is_number()) {
		throw std::runtime_error("Rigid body node has no valid 'mass' value.");
	}

	World::RigidBody* rigid_body;

	auto type = type_it->get<std::string>();
	if (type == "Box") {
		auto size = nlohmann::parseVector3f(json, "size", Eigen::Vector3f::Zero());
		if (size.isZero()) {
			throw std::runtime_error("Box rigid body has invalid size value.");
		}
		rigid_body = &scene_.emplace<World::BoxRigidBody>(entity, mass_it->get<float>(), size);
	} else if (type == "Capsule") {
		auto radius = nlohmann::parseFloat(json, "radius", -1.0f);
		auto height = nlohmann::parseFloat(json, "height", -1.0f);
		if (radius < 0.0f || height < 0.0f) {
			throw std::runtime_error("Capsule rigid body has invalid radius or height value.");
		}
		rigid_body = &scene_.emplace<World::CapsuleRigidBody>(entity, mass_it->get<float>(), radius, height);
	} else if (type == "Cone") {
		auto radius = nlohmann::parseFloat(json, "radius", -1.0f);
		auto height = nlohmann::parseFloat(json, "height", -1.0f);
		if (radius < 0.0f || height < 0.0f) {
			throw std::runtime_error("Capsule rigid body has invalid radius or height value.");
		}
		rigid_body = &scene_.emplace<World::ConeRigidBody>(entity, mass_it->get<float>(), radius, height);
	} else if (type == "Cylinder") {
		auto size = nlohmann::parseVector3f(json, "size", Eigen::Vector3f::Zero());
		if (size.isZero()) {
			throw std::runtime_error("Box rigid body has invalid size value.");
		}
		rigid_body = &scene_.emplace<World::CylinderRigidBody>(entity, mass_it->get<float>(), size);
	} else if (type == "StaticPlane") {
		auto radius = nlohmann::parseFloat(json, "radius", -1.0f);
		if (radius < 0.0f) {
			throw std::runtime_error("Sphere rigid body has invalid radius value.");
		}
		rigid_body = &scene_.emplace<World::SphereRigidBody>(entity, mass_it->get<float>(), radius);
	} else if (type == "TriangleMesh") {
		// TODO
		scene_.emplace<World::TriangleMeshRigidBody>(entity, mass_it->get<float>());
	} else {
		throw std::runtime_error(std::format("Unknown rigid body type '{}'", type));
	}

	rigid_body->localTransform() = nlohmann::parseAffine3f(json, "offset");
}

VOLCANO_NET_END
