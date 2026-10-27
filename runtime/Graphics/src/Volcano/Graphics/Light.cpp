#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>

#include <GL/gl.hpp>

#include <Volcano/Graphics/Light.h>
#include <Volcano/Math.h>

VOLCANO_GRAPHICS_BEGIN

void Light::Uniforms::resolve(const gl::Program& program, std::size_t index) {
    const auto prefix = "uLights[" + std::to_string(index) + "].";
    type = program.uniformLocation((prefix + "type").c_str(), GL_INT);
    radiance = program.uniformLocation((prefix + "radiance").c_str(), GL_FLOAT_VEC3);
    position = program.uniformLocation((prefix + "position").c_str(), GL_FLOAT_VEC3);
    direction = program.uniformLocation((prefix + "direction").c_str(), GL_FLOAT_VEC3);
    range = program.uniformLocation((prefix + "range").c_str(), GL_FLOAT);
    cone = program.uniformLocation((prefix + "cone").c_str(), GL_FLOAT_VEC2);
}

namespace {

constexpr const char* LightingSource = R"GLSL(
struct Light {
    int type;
    vec3 radiance;
    vec3 position;
    vec3 direction;
    float range;
    vec2 cone;
};
uniform Light uLights[MAX_LIGHTS];
uniform int uLightCount;
uniform vec3 uAmbient;
vec3 safeNormalize(vec3 value) {
    return value / max(length(value), 1e-20);
}

vec3 evaluateLighting(vec3 position, vec3 worldNormal) {
    vec3 normal = safeNormalize(worldNormal);
    vec3 illumination = uAmbient;
    for (int i = 0; i < uLightCount; ++i) {
        Light light = uLights[i];
        vec3 toLight = -light.direction;
        float attenuation = 1.0;
        if (light.type != 0) {
            vec3 delta = light.position - position;
            float distanceToLight = length(delta);
            toLight = safeNormalize(delta);
            float falloff = max(1.0 - distanceToLight / light.range, 0.0);
            attenuation = falloff * falloff;
            if (light.type == 2) {
                float cone = dot(light.direction, -toLight);
                attenuation *= smoothstep(light.cone.y, light.cone.x, cone);
            }
        }

        illumination += light.radiance * max(dot(normal, toLight), 0.0) * attenuation;
    }

    return illumination;
}
)GLSL";

}

void Light::validate() const {
    if (!color.allFinite() || (color.array() < 0.0f).any() ||
        !std::isfinite(strength) || strength < 0.0f ||
        !(color * strength).allFinite()) {
        throw std::invalid_argument("Light radiance must be finite and nonnegative");
    }

    switch (type) {
    case Light::Type::Directional:
    case Light::Type::Point:
    case Light::Type::Spot:
        break;
    default:
        throw std::invalid_argument("Unknown light type");
    }
    if (type != Light::Type::Point &&
        (!direction.allFinite() || !std::isfinite(direction.stableNorm()) ||
         direction.stableNorm() == 0.0f)) {
        throw std::invalid_argument("Light direction must be finite and nonzero");
    }
    if (type != Light::Type::Directional &&
        (!position.allFinite() || !std::isfinite(range) || range <= 0.0f)) {
        throw std::invalid_argument("Local lights require a finite position and positive finite range");
    }
    if (type == Light::Type::Spot &&
        (!std::isfinite(innerAngle) || !std::isfinite(outerAngle) ||
         innerAngle < 0.0f || innerAngle >= outerAngle || outerAngle > 90.0f)) {
        throw std::invalid_argument("Spot half-angles require 0 <= inner < outer <= 90 degrees");
    }
    if (type == Light::Type::Spot &&
        std::cos(Eigen::radians(innerAngle)) <= std::cos(Eigen::radians(outerAngle))) {
        throw std::invalid_argument("Spot cone angles must have distinct cosine values");
    }
}

std::string Light::shaderSource(std::size_t maxLights) {
    if (maxLights == 0 || maxLights > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        throw std::invalid_argument("Light capacity must be a positive GLSL int");
    }
    return "#define MAX_LIGHTS " + std::to_string(maxLights) + "\n" + LightingSource;
}

Light::Prepared::Prepared(const Light& light) {
    light.validate();
    type_ = light.type;
    radiance_ = light.color * light.strength;
    position_ = light.position;
    direction_ = Eigen::Vector3f::Zero();
    if (type_ != Type::Point) {
        direction_ = light.direction / light.direction.stableNorm();
    }
    range_ = light.range;
    cone_ = Eigen::Vector2f::Zero();
    if (type_ == Type::Spot) {
        cone_ = {std::cos(Eigen::radians(light.innerAngle)), std::cos(Eigen::radians(light.outerAngle))};
    }
}

void Light::Prepared::apply(const gl::Program& program, const Uniforms& uniforms) const {
    program.uniform(uniforms.type, static_cast<GLint>(type_));
    program.uniform3f(uniforms.radiance, radiance_.x(), radiance_.y(), radiance_.z());
    program.uniform3f(uniforms.position, position_.x(), position_.y(), position_.z());
    program.uniform3f(uniforms.direction, direction_.x(), direction_.y(), direction_.z());
    program.uniform(uniforms.range, range_);
    if (type_ == Type::Spot) {
        program.uniform2f(uniforms.cone, cone_.x(), cone_.y());
    }
}

VOLCANO_GRAPHICS_END
