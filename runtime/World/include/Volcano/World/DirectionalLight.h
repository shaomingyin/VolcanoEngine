//
//
#ifndef VOLCANO_WORLD_DIRECTIONALLIGHT_H
#define VOLCANO_WORLD_DIRECTIONALLIGHT_H

#include <Volcano/World/Common.h>
#include <Volcano/World/Light.h>

VOLCANO_WORLD_BEGIN

class DirectionalLight: public Light {
public:
    DirectionalLight() noexcept;
    DirectionalLight(const Volcano::Math::Vector3f& direction, const Volcano::Math::Vector3f& color, float strength = 1.0f) noexcept;
    DirectionalLight(float dx, float dy, float dz, float r, float g, float b, float strength = 1.0f) noexcept;
    DirectionalLight(const DirectionalLight&) noexcept = default;
    DirectionalLight(DirectionalLight&&) noexcept = default;

public:
    DirectionalLight& operator=(const DirectionalLight&) noexcept = default;
    DirectionalLight& operator=(DirectionalLight&&) noexcept = default;

    Volcano::Math::Vector3f& direction() noexcept {
        return direction_;
    }

    const Volcano::Math::Vector3f& direction() const noexcept {
        return direction_;
    }

private:
    Volcano::Math::Vector3f direction_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_DIRECTIONALLIGHT_H
