//
//
#include <vector>
#include <thread>

#include <Volcano/ScopeGuard.h>
#include <Volcano/Player/Base.h>

VOLCANO_PLAYER_BEGIN

Base::Base(World::Scene& scene)
	: scene_(scene)
	, state_(State::Idle)
	, loading_progress_(0)
	, frame_count_(0)
	, frame_count_per_second_(0) {
}

void Base::run(const std::string& path) {
	path_ = path;
	frame_last_ = Clock::now();
	frame_count_last_ = frame_last_;
	frame_count_ = 0;
	frame_count_per_second_ = 0;

	load();

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
				logInfo("{}fps", frame_count_per_second_);
			}
		} else {
			auto left = elapsed_min_ - elapsed;
			std::this_thread::sleep_for(left < 10ms ? left : 10ms);
		}
	}
}

void Base::load() {
	if (state_ == State::Loading) {
		return;
	}

	name_.clear();
	scene_.clear();
	loading_message_.clear();
	loading_progress_ = 0;

	if (!path_.empty()) {
		state_ = State::Loading;
		loading_task_ = async::spawn([this] {
			loading_message_ = "Loading scene...";
			loadScene(nlohmann::parseFromPhysFS(path_));
		});
	} else {
		state_ = State::Ready;
	}
}

void Base::loadScene(const nlohmann::json& json) {
	name_ = nlohmann::parseString(json, "name");

	if (nlohmann::parseBool(json, "physicsEnabled")) {
		scene_.enablePhysics();
	} else {
		scene_.disablePhysics();
	}

	scene_.setGravity(nlohmann::parseVector3f(json, "gravity", Eigen::Vector3f(0.0f, -9.8f, 0.0f)));

	auto entities_it = json.find("entities");
	if (entities_it != json.end()) {
		loading_message_ = "Loading entities...";
		if (!entities_it->is_array()) {
			throw std::runtime_error("Node 'entities' in scene file is not an array.");
		}
		auto n = entities_it->size();
		for (size_t i = 0; i < n; ++i) {
			loading_progress_ = i * 100 / n;
			loadEntity(entities_it->at(i), scene_.createEntity());
		}
	}

	loading_progress_ = 100;
}

void Base::loadEntity(const nlohmann::json& json, entt::handle entity) {
	auto& inherency = entity.emplace<World::Inherency>(nlohmann::parseString(json, "name"));

	if (nlohmann::parseBool(json, "enabled", true)) {
		inherency.enable();
	} else {
		inherency.disable();
	}

	auto translation = nlohmann::parseVector3f(json, "translation");
	auto scaling = nlohmann::parseVector3f(json, "scaling");
	auto aa = nlohmann::parseVector4f(json, "rotation");
	Eigen::Quaternionf rotation(aa.w(), aa.x(), aa.y(), aa.z());

	auto transform = Eigen::Affine3f::Identity();
	transform.translate(translation);
	transform.scale(scaling);
	transform.rotate(rotation);
	inherency.transform() = transform;

	// TODO load physics components
}

void Base::clear() {
	path_.clear();
	name_.clear();
	scene_.clear();
	loading_message_.clear();
	loading_progress_ = 0;
	error_message_.clear();
	state_ = State::Idle;
}

void Base::loadingFrame(Duration elapsed) {
}

void Base::readyFrame(Duration elapsed) {
}

void Base::playingFrame(Duration elapsed) {
	scene_.update(elapsed);
}

void Base::pausedFrame(Duration elapsed) {
}

void Base::stoppingFrame(Duration elapsed) {
	clear();
}

void Base::errorFrame(Duration elapsed) {
}

void Base::frame(Duration elapsed) {
	switch (state_) {
	case State::Playing:
		playingFrame(elapsed);
		break;
	case State::Loading:
		if (loading_task_.ready()) {
			try {
				loading_task_.get();
				state_ = State::Ready;
			} catch (std::exception& e) {
				error(e.what());
			}
		} else {
			loadingFrame(elapsed);
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
	case State::Error:
		errorFrame(elapsed);
		break;
	default:
		break;
	}
}

VOLCANO_PLAYER_END
