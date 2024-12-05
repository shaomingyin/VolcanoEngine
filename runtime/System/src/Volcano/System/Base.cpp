//
//
#include <vector>
#include <thread>

#include <Volcano/ScopeGuard.h>
#include <Volcano/World/BoxRigidBody.h>
#include <Volcano/World/CapsuleRigidBody.h>
#include <Volcano/World/ConeRigidBody.h>
#include <Volcano/World/CylinderRigidBody.h>
#include <Volcano/World/SphereRigidBody.h>
#include <Volcano/World/StaticPlaneRigidBody.h>
#include <Volcano/World/TriangleMeshRigidBody.h>
#include <Volcano/System/Base.h>

VOLCANO_SYSTEM_BEGIN

Base::Base(const std::string& manifest_path)
	: state_(State::Idle)
	, threadpool_(async::hardware_concurrency())
	, manifest_path_(manifest_path)
	, loading_progress_(0)
	, frame_count_(0)
	, frame_count_per_second_(0) {
	setFpsMax(60);
}

void Base::run() {
	if (state_ != State::Idle) {
		throw std::runtime_error("Invalid state.");
	}

	state_ = State::Loading;
	loading_task_ = async::spawn(async::thread_scheduler(), std::bind(&Base::load, this));

	frame_last_ = Clock::now();
	frame_count_last_ = frame_last_;
	frame_count_ = 0;
	frame_count_per_second_ = 0;

	while (state_ != State::Idle) {
		auto current = Clock::now();
		auto elapsed = current - frame_last_;
		if (elapsed >= elapsed_min_) {
			frame(elapsed);
			frame_count_ += 1;
			frame_last_ = current;
			if ((current - frame_count_last_) >= 1s) {
				frame_count_per_second_ = frame_count_;
				frame_count_ = 0;
				frame_count_last_ = current;
				logInfo("Simulation: {}fps", frame_count_per_second_);
			}
		} else {
			auto left = elapsed_min_ - elapsed;
			std::this_thread::sleep_for(left < 10ms ? left : 10ms);
		}
	}
}

void Base::loadScene(const nlohmann::json& json) {
	loading_message_ = "Loading scene...";

	name_ = nlohmann::parseString(json, "name");

	auto map_it = json.find("map");
	if (map_it == json.end() || !map_it->is_object()) {
		throw std::runtime_error("Scene has no map node.");
	}

	loadMap(*map_it, scene_.global());

	if (nlohmann::parseBool(json, "physicsEnabled")) {
		scene_.enablePhysics();
	} else {
		scene_.disablePhysics();
	}

	auto gravity = nlohmann::parseVector3f(json, "gravity", Eigen::Vector3f(0.0f, -9.8f, 0.0f));
	scene_.setGravity(gravity);

	auto entities_it = json.find("entities");
	if (entities_it != json.end()) {
		loading_message_ = "Loading entities...";
		if (!entities_it->is_array()) {
			throw std::runtime_error("Node 'entities' in scene file is not an array.");
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

void Base::clear() {
	name_.clear();
	scene_.clear();
	loading_message_.clear();
	loading_progress_ = 0;
	error_message_.clear();
	state_ = State::Idle;
}

void Base::loadingFrame(Duration elapsed) {
	if (state_ == State::Loading) {
		if (loading_task_.ready()) {
			try {
				loading_task_.get();
				state_ = State::Ready;
			} catch (std::exception& ex) {
				error(ex.what());
			}
		}
	}
}

void Base::readyFrame(Duration elapsed) {
}

void Base::playingFrame(Duration elapsed) {
	scene_.update(elapsed);
}

void Base::pausedFrame(Duration elapsed) {
}

void Base::errorFrame(Duration elapsed) {
}


void Base::frame(Duration elapsed) {
	task_scheduler_.run_all_tasks();
	switch (state_) {
	case State::Playing:
		playingFrame(elapsed);
		break;
	case State::Loading:
		loadingFrame(elapsed);
		break;
	case State::Ready:
		readyFrame(elapsed);
		break;
	case State::Paused:
		pausedFrame(elapsed);
		break;
	case State::Error:
		errorFrame(elapsed);
		break;
	default:
		break;
	}
}

void Base::load() {
	VOLCANO_ASSERT(state_ == State::Loading);

	name_.clear();
	scene_.clear();
	loading_message_.clear();
	loading_progress_ = 0;
	loading_cancellation_.reset();

	auto manifest = nlohmann::parseFromPhysFS(manifest_path_.empty() ? "/Manifest.json" : manifest_path_);
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

VOLCANO_SYSTEM_END
