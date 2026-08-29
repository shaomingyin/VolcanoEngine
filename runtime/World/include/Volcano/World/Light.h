//
//
#ifndef VOLCANO_WORLD_LIGHT_H
#define VOLCANO_WORLD_LIGHT_H

#include <Volcano/Math.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

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

private:
    Transform offset_;
    Eigen::Vector3f color_;
    float strength_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_LIGHT_H
