//
//
#ifndef VOLCANO_WORLD_POINTLIGHT_H
#define VOLCANO_WORLD_POINTLIGHT_H

#include <Volcano/World/Common.h>
#include <Volcano/World/Light.h>

VOLCANO_WORLD_BEGIN

class PointLight: public Light {
public:
    PointLight() noexcept;
    PointLight(const Eigen::Vector3f& color, float strength, float range) noexcept;
    PointLight(float r, float g, float b, float strength, float range) noexcept;
    PointLight(const PointLight&) noexcept = default;
    PointLight(PointLight&&) noexcept = default;

public:
    PointLight& operator=(const PointLight&) noexcept = default;
    PointLight& operator=(PointLight&&) noexcept = default;

    float& range() noexcept {
        return range_;
    }

    const float& range() const noexcept {
        return range_;
    }

private:
    float range_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_POINTLIGHT_H

