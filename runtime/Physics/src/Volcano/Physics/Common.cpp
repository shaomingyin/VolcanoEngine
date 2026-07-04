//
//
#include <memory>

#include <Jolt/Jolt.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/CylinderShape.h>
#include <Jolt/Physics/Collision/Shape/PlaneShape.h>

#include <Volcano/Math.h>
#include <Volcano/Physics/Common.h>

namespace JPH {
	nlohmann::json toJson(const Shape& v) {
		auto json = nlohmann::json::object();
		switch (v.GetType()) {
		case JPH::EShapeType::Convex:
			json["type"] = "convex";
			switch (v.GetSubType()) {
			case JPH::EShapeSubType::Box:
				json["subtype"] = "box";
				break;
			case JPH::EShapeSubType::Capsule:
				json["subtype"] = "capsule";
				break;
			case JPH::EShapeSubType::Cylinder:
				json["subtype"] = "cylinder";
				break;
			case JPH::EShapeSubType::Plane:
				json["subtype"] = "plane";
				break;
			default:
				throw std::invalid_argument("Invalid JSON format for JPH.Shape.SubType");
				break;
			}
			break;
		case JPH::EShapeType::Compound:
			json["type"] = "compound";
			break;
		default:
			throw std::invalid_argument("Invalid JSON format for JPH.Shape.Type");
			break;
		}
		return json;
	}

	Shape* fromJson(const nlohmann::json& json) {
		if (json.is_object()) {
			std::unique_ptr<Shape> shape;
			auto type = json["type"];
			auto subtype = json["subtype"];
			if (type == "convex") {
				if (subtype == "box") {
					auto size = json["size"].get<Eigen::Vector3f>();
					shape = std::make_unique<BoxShape>(size.x(), size.y(), size.z());
				} else if (subtype == "capsule") {
					// TODO
					shape = std::make_unique<CapsuleShape>();
				} else if (subtype == "cylinder") {
					// TODO
					shape = std::make_unique<CylinderShape>();
				} else if (subtype == "plane") {
					// TODO
					shape = std::make_unique<PlaneShape>();
				} else {
					throw std::invalid_argument("Invalid JSON format for JPH.Shape.SubType");
				}
			}
			return shape.release();
		} else {
			throw std::invalid_argument("Invalid JSON format for JPH.Shape");
		}
	}

	void to_json(nlohmann::json& json, const BodyCreationSettings& v) {
		json = nlohmann::json::object();
		json["shape"] = toJson(*v.GetShape());
		Volcano::Transform transform;
		transform.translation() = Volcano::Physics::fromJPH(v.mPosition);
		transform.rotation() = Volcano::Physics::fromJPH(v.mRotation);
		json["transform"] = transform;
	}

	void from_json(const nlohmann::json& json, BodyCreationSettings& v) {
		if (json.is_object()) {
			auto transform = json["transform"].get<Volcano::Transform>();
			v.mPosition = Volcano::Physics::toJPH(transform.translation());
			v.mRotation = Volcano::Physics::toJPH(transform.rotation());
			v.SetShape(fromJson(json["shape"]));
		} else {
			throw std::invalid_argument("Invalid JSON format for JPH.BodyCreationSettings");
		}
	}
}

VOLCANO_PHYSICS_BEGIN

void init() {
	spdlog::info("Initializing physics system...");
	JPH::RegisterDefaultAllocator();
	JPH::Factory::sInstance = new JPH::Factory();
	JPH::RegisterTypes();
}

void shutdown() {
	spdlog::info("Shutting down physics system...");
	delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}

VOLCANO_PHYSICS_END
