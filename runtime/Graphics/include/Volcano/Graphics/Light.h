//
//
#ifndef VOLCANO_GRAPHICS_LIGHT_H
#define VOLCANO_GRAPHICS_LIGHT_H

#include <Volcano/Math.h>
#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Light {
public:
    Light() noexcept;
    Light(const Eigen::Vector3f& color, float strength = 1.0f) noexcept;
    Light(float r, float g, float b, float strength = 1.0f) noexcept;
    Light(const Light&) noexcept = default;
    Light(Light&&) noexcept = default;

public:
    Light& operator=(const Light&) noexcept = default;
    Light& operator=(Light&&) noexcept = default;

    Transform& offset() noexcept {
        return offset_;
    }

    const Transform& offset() const noexcept {
        return offset_;
    }

    Eigen::Vector3f& color() noexcept {
        return color_;
    }

    const Eigen::Vector3f& color() const noexcept {
        return color_;
    }

    float& strength() noexcept {
        return strength_;
    }

    const float& strength() const noexcept {
        return strength_;
    }

    friend void to_json(nlohmann::json& json, const Light& v);
    friend void from_json(const nlohmann::json& json, Light& v);

private:
    Transform offset_;
    Eigen::Vector3f color_;
    float strength_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_LIGHT_H
