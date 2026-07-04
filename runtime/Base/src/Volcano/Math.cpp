//
//
#include <Volcano/Math.h>

namespace Eigen {
    void to_json(nlohmann::json& json, const Eigen::Vector2f& v) {
        json = nlohmann::json::array();
        json.push_back(v.x());
        json.push_back(v.y());
    }

    void from_json(const nlohmann::json& json, Eigen::Vector2f& v) {
        if (json.is_array() && json.size() == 2) {
            v.x() = json[0].get<float>();
            v.y() = json[1].get<float>();
        }
        else {
            throw std::invalid_argument("Invalid JSON format for Eigen::Vector2f");
        }
    }

    void to_json(nlohmann::json& json, const Eigen::Vector3f& v) {
        json = nlohmann::json::array();
        json.push_back(v.x());
        json.push_back(v.y());
        json.push_back(v.z());
    }

    void from_json(const nlohmann::json& json, Eigen::Vector3f& v) {
        if (json.is_array() && json.size() == 3) {
            v.x() = json[0].get<float>();
            v.y() = json[1].get<float>();
            v.z() = json[2].get<float>();
        }
        else {
            throw std::invalid_argument("Invalid JSON format for Eigen::Vector3f");
        }
    }

    void to_json(nlohmann::json& json, const Eigen::Vector4f& v) {
        json = nlohmann::json::array();
        json.push_back(v.x());
        json.push_back(v.y());
        json.push_back(v.z());
        json.push_back(v.w());
    }

    void from_json(const nlohmann::json& json, Eigen::Vector4f& v) {
        if (json.is_array() && json.size() == 4) {
            v.x() = json[0].get<float>();
            v.y() = json[1].get<float>();
            v.z() = json[2].get<float>();
            v.w() = json[3].get<float>();
        }
        else {
            throw std::invalid_argument("Invalid JSON format for Eigen::Vector4f");
        }
    }

    void to_json(nlohmann::json& json, const Eigen::Quaternionf& v) {
        json = nlohmann::json::array();
        json.push_back(v.w());
        json.push_back(v.x());
        json.push_back(v.y());
        json.push_back(v.z());
    }

    void from_json(const nlohmann::json& json, Eigen::Quaternionf& v) {
        if (json.is_array() && json.size() == 4) {
            v.w() = json[0].get<float>();
            v.x() = json[1].get<float>();
            v.y() = json[2].get<float>();
            v.z() = json[3].get<float>();
        }
        else {
            throw std::invalid_argument("Invalid JSON format for Eigen::Quaternionf");
        }
    }

    void to_json(nlohmann::json& json, const Eigen::Matrix3f& v) {
        json = nlohmann::json::array();
        for (int i = 0; i < 9; ++i) {
            json.push_back(v.data()[i]);
        }
    }

    void from_json(const nlohmann::json& json, Eigen::Matrix3f& v) {
        if (json.is_array() && json.size() == 9) {
            for (int i = 0; i < 9; ++i) {
                v.data()[i] = json[i].get<float>();
            }
        }
        else {
            throw std::invalid_argument("Invalid JSON format for Eigen::Matrix3f");
        }
    }

    void to_json(nlohmann::json& json, const Eigen::Matrix4f& v) {
        json = nlohmann::json::array();
        for (int i = 0; i < 16; ++i) {
            json.push_back(v.data()[i]);
        }
    }

    void from_json(const nlohmann::json& json, Eigen::Matrix4f& v) {
        if (json.is_array() && json.size() == 16) {
            for (int i = 0; i < 16; ++i) {
                v.data()[i] = json[i].get<float>();
            }
        }
        else {
            throw std::invalid_argument("Invalid JSON format for Eigen::Matrix4f");
        }
    }

    void to_json(nlohmann::json& json, const Eigen::Affine3f& v) {
        json = nlohmann::json::array();
        for (int i = 0; i < 16; ++i) {
            json.push_back(v.matrix().data()[i]);
        }
    }

    void from_json(const nlohmann::json& json, Eigen::Affine3f& v) {
        if (json.is_array() && json.size() == 16) {
            Eigen::Matrix4f m;
            for (int i = 0; i < 16; ++i) {
                m.data()[i] = json[i].get<float>();
            }
            v = Eigen::Affine3f(m);
        }
        else {
            throw std::invalid_argument("Invalid JSON format for Eigen::Affine3f");
        }
    }
}

VOLCANO_BEGIN

void to_json(nlohmann::json& json, const Transform& v) {
    json = nlohmann::json::object();
    json["translation"] = v.translation_;
    json["scale"] = v.scale_;
    json["rotation"] = v.rotation_;
}

void from_json(const nlohmann::json& json, Transform& v) {
    if (json.is_object() && json.contains("translation") && json.contains("scale") && json.contains("rotation")) {
        v.translation_ = json["translation"].get<Eigen::Vector3f>();
        v.scale_ = json["scale"].get<Eigen::Vector3f>();
        v.rotation_ = json["rotation"].get<Eigen::Quaternionf>();
    } else {
        throw std::invalid_argument("Invalid JSON format for Transform");
    }
}

VOLCANO_END
