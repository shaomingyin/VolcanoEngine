//
//
#ifndef VOLCANO_GRAPHICS_SPOTLIGHT_H
#define VOLCANO_GRAPHICS_SPOTLIGHT_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/DirectionalLight.h>

VOLCANO_GRAPHICS_BEGIN

class SpotLight: public DirectionalLight {
public:
    SpotLight() noexcept;
    SpotLight(const Eigen::Vector3f& direction, const Eigen::Vector3f& color, float strength, float inner_angle, float outer_angle) noexcept;
    SpotLight(float dx, float dy, float dz, float r, float g, float b, float strength, float inner_angle, float outer_angle) noexcept;
    SpotLight(const SpotLight&) noexcept = default;
    SpotLight(SpotLight&&) noexcept = default;

public:
    SpotLight& operator=(const SpotLight&) noexcept = default;
    SpotLight& operator=(SpotLight&&) noexcept = default;

    float& innerAngle() noexcept {
        return inner_angle_;
    }

    const float& innerAngle() const noexcept {
        return inner_angle_;
    }

    float& outerAngle() noexcept {
        return outer_angle_;
    }

    const float& outerAngle() const noexcept {
        return outer_angle_;
    }

private:
    float inner_angle_;
	float outer_angle_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_SPOTLIGHT_H
