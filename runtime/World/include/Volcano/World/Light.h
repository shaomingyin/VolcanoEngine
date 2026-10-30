#ifndef VOLCANO_WORLD_LIGHT_H
#define VOLCANO_WORLD_LIGHT_H

#include <Eigen/Core>

#include <Volcano/Transform.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Light {
public:
    virtual ~Light() = default;

    constexpr const Transform& offset() const noexcept {
        return offset_;
    }

    void setOffset(const Transform& value) noexcept {
        offset_ = value;
    }

    constexpr const Eigen::Vector3f& color() const noexcept {
        return color_;
    }

    void setColor(const Eigen::Vector3f& value);

    constexpr float strength() const noexcept {
        return strength_;
    }

    void setStrength(float value);

protected:
    Light() noexcept;
    Light(const Eigen::Vector3f& color, float strength = 1.0f);
    Light(float r, float g, float b, float strength = 1.0f);
    Light(const Light&) = default;
    Light(Light&&) = default;
    Light& operator=(const Light&) = default;
    Light& operator=(Light&&) = default;

private:
    Transform offset_;
    Eigen::Vector3f color_{1.0f, 1.0f, 1.0f};
    float strength_ = 1.0f;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_LIGHT_H
