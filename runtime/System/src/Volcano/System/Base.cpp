//
//
#include <thread>

#include <Volcano/Error.h>
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

Base::Base(const std::string& manifest_path, int sdl_init_flags)
	: manifest_path_(manifest_path)
	, sdl_init_result_(SDL_Init(sdl_init_flags))
	, state_(State::Idle)
	, loading_progress_(0) {
	if (sdl_init_result_ != 0) {
		throw Error(Errc::OutOfResource);
	}
	setMaxFps(60);
}

Base::~Base() {
	SDL_Quit();
}

void Base::run() {
	startLoading();

	frame_count_ = 0;
	frame_count_per_second_ = 0;
	last_frame_timestamp_ = Clock::now();
	last_frame_count_timestamp_ = last_frame_timestamp_;

	while (state_ != State::Idle) {
		pollEvents();
		TimePoint curr = Clock::now();
		auto elapsed = curr - last_frame_timestamp_;
		if (elapsed < frame_elapsed_min_) {
			auto delay = frame_elapsed_min_ - elapsed;
			if (delay > 20ms) {
				std::this_thread::sleep_for(20ms);
			} else {
				std::this_thread::sleep_for(delay);
			}
			continue;
		}
		frame(elapsed);
		frame_count_ += 1;
		last_frame_timestamp_ = curr;
		if ((curr - last_frame_count_timestamp_) >= std::chrono::seconds(1)) {
			frame_count_per_second_ = frame_count_;
			frame_count_ = 0;
			last_frame_count_timestamp_ = curr;
			logDebug("FPS: {}", frame_count_per_second_);
		}
	}
}

void Base::frame(Duration elapsed) {
	scene_.frame(elapsed);

	switch (state_) {
	case State::Playing:
		playingFrame(elapsed);
		break;
	case State::Loading:
		loadingFrame(elapsed);
		if (loading_future_.wait_for(0s) == std::future_status::ready) {
			loading_future_.get();
			state_ = State::Ready;
		}
		break;
	case State::Ready:
		readyFrame(elapsed);
		break;
	case State::Paused:
		pausedFrame(elapsed);
		break;
	case State::Stopping:
		stoppingFrame(elapsed);
		break;
	default:
		errorFrame(elapsed);
		break;
	}
}

void Base::pollEvents() {
	if (SDL_WasInit(SDL_INIT_EVENTS)) {
		SDL_Event evt;
		while (SDL_PollEvent(&evt)) {
			if (evt.type != SDL_QUIT) {
				handleEvent(evt);
			} else {
				state_ = State::Stopping;
			}
		}
	}
}

void Base::startLoading() {
	if (state_ != State::Idle && state_ != State::Playing && state_ != State::Paused) {
		throw Error(Errc::InvalidState);
	}

	state_ = State::Loading;

	loading_future_ = defaultExecutor.async([this]() mutable {
		loadManifest();
	});
}

void Base::loadManifest() {
	if (state_ != State::Loading) {
		throw Error(Errc::InvalidState);
	}

	loading_text_ = "Loading manifest...";
	auto manifest = loadJson(manifest_path_);

	auto name_it = manifest.find("name");
	if (name_it != manifest.end()) {

	}

	auto description_it = manifest.find("description");
	if (description_it != manifest.end()) {

	}

	auto version_it = manifest.find("version");
	if (version_it != manifest.end()) {

	}

	auto engine_version_it = manifest.find("engineVersion");
	if (engine_version_it != manifest.end()) {

	}

	auto entry_it = manifest.find("entry");
	if (entry_it == manifest.end()) {
		throw Error(Errc::NotExisted);
	}

	loadScene(entry_it->get<std::string>());
}

void Base::loadScene(const std::string& path) {
	auto json = loadJson(path);
	if (!json.is_object()) {
		throw Error(Errc::InvalidType);
	}

	auto name_it = json.find("name");
	if (name_it == json.end()) {
		throw Error(Errc::NotExisted);
	}

	auto physics_it = json.find("physics");
	if (physics_it != json.end() && physics_it->is_boolean()) {
		if (physics_it->get<bool>()) {
			scene().enablePhysics();
		} else {
			scene().disablePhysics();
		}
	}

	auto gravity_it = json.find("gravity");
	if (gravity_it != json.end()) {
		if (!gravity_it->is_array() || gravity_it->size() != 3) {
			throw Error(Errc::InvalidType);
		}
		scene().setGravity(parseVector3f(*gravity_it));
	}

	auto next_it = json.find("next");
	if (next_it != json.end()) {
	}

	auto entities_it = json.find("entities");
	if (entities_it != json.end()) {
		if (!entities_it->is_array()) {
			throw Error(Errc::InvalidType);
		}
		loading_text_ = "Loading entities...";
		size_t n = entities_it->size();
		for (size_t i = 0; i < n; ++i) {
			loading_progress_ = i * 100 / n;
			loadEntity(scene().emplaceEntity(), entities_it->at(i));
		}
	}
}

void Base::loadEntity(World::Entity ent, const nlohmann::json& json) {
	if (!json.is_object()) {
		throw Error(Errc::InvalidType);
	}

	std::string name;
	auto name_it = json.find("name");
	if (name_it == json.end()) {
		throw Error(Errc::NotExisted);
	}
	ent.name() = name_it->get<std::string>();
	loading_text_ = std::format("Loading entity {}...", ent.name());

	ent.transform() = parseTransform(json);

	auto rigid_body_it = json.find("rigid_body");
	if (rigid_body_it != json.end()) {
		if (!rigid_body_it->is_object()) {
			throw Error(Errc::InvalidType);
		}
		auto mass_it = rigid_body_it->find("mass");
		if (mass_it == rigid_body_it->end() || !mass_it->is_number_float()) {
			throw Error(Errc::NotExisted);
		}
		auto mass = mass_it->get<float>();
		auto shape_it = rigid_body_it->find("shape");
		if (shape_it == rigid_body_it->end() || !shape_it->is_string()) {
			throw Error(Errc::NotExisted);
		}
		World::RigidBody* rigid_body = nullptr;
		auto shape = shape_it->get<std::string>();
		if (shape == "Box") {
			auto box_size_it = rigid_body_it->find("size");
			if (box_size_it == rigid_body_it->end() || !box_size_it->is_array() || box_size_it->size() != 3) {
				throw Error(Errc::InvalidType);
			}
			rigid_body = &ent.emplaceOrReplace<World::BoxRigidBody>(mass, parseVector3f(*box_size_it));
		}
		else if (shape == "Capsule") {

		}
		else if (shape == "Cone") {

		}
		else if (shape == "Cylinder") {

		}
		else if (shape == "Sphere") {

		}
		else if (shape == "StaticPlane") {

		}
		else if (shape == "TriangleMesh") {

		}
		else {
			throw Error(Errc::InvalidType);
		}
		auto offset_it = rigid_body_it->find("offset");
		if (offset_it != rigid_body_it->end()) {
			rigid_body->offset() = parseTransform(*offset_it);
		}
	}
}

nlohmann::json Base::loadJson(const std::string& path) {
	PHYSFS_File* fp = PHYSFS_openRead(path.c_str());
	if (fp == nullptr) {
		throw Error(Errc::NotExisted);
	}

	nlohmann::json json;
	PHYSFS_sint64 file_size = PHYSFS_fileLength(fp);
	if (file_size > 0) {
		Buffer manifest_data(file_size);
		PHYSFS_sint64 read_size = PHYSFS_readBytes(fp, manifest_data.data(), file_size);
		if (read_size != file_size) {
			throw Error(Errc::InvalidParameter);
		}
		json = nlohmann::json::parse(manifest_data);
	}

	return json;
}

Eigen::Affine3f Base::parseTransform(const nlohmann::json& json) {
	auto transform(Eigen::Affine3f::Identity());

	auto position_it = json.find("position");
	if (position_it != json.end()) {
		transform.translate(parseVector3f(*position_it));
	}

	auto scaling_it = json.find("scaling");
	if (scaling_it != json.end()) {
		transform.scale(parseVector3f(*scaling_it));
	}

	auto rotation_it = json.find("rotation");
	if (rotation_it != json.end()) {
		transform.rotate(Eigen::AngleAxisf(parseVector4f(*rotation_it)));
	}

	return transform;
}

Eigen::Vector3f Base::parseVector3f(const nlohmann::json& json) {
	Eigen::Vector3f ret;
	if (json.is_array() && json.size() == 3) {
		ret.x() = json.at(0).get<double>();
		ret.y() = json.at(1).get<double>();
		ret.z() = json.at(2).get<double>();
	} else {
		throw Error(Errc::InvalidType);
	}
	return ret;
}

Eigen::Vector4f Base::parseVector4f(const nlohmann::json& json) {
	Eigen::Vector4f ret;
	if (json.is_array() && json.size() == 4) {
		ret.x() = json.at(0).get<double>();
		ret.y() = json.at(1).get<double>();
		ret.z() = json.at(2).get<double>();
		ret.w() = json.at(3).get<double>();
	} else {
		throw Error(Errc::InvalidType);
	}
	return ret;
}

VOLCANO_SYSTEM_END
