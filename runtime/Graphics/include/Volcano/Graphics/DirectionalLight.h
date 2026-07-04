//
//
#ifndef VOLCANO_GRAPHICS_DIRECTIONALLIGHT_H
#define VOLCANO_GRAPHICS_DIRECTIONALLIGHT_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Light.h>

VOLCANO_GRAPHICS_BEGIN

class DirectionalLight: public Light {
public:
    DirectionalLight() noexcept;
    DirectionalLight(const Eigen::Vector3f& direction, const Eigen::Vector3f& color, float strength = 1.0f) noexcept;
    DirectionalLight(float dx, float dy, float dz, float r, float g, float b, float strength = 1.0f) noexcept;
    DirectionalLight(const DirectionalLight&) noexcept = default;
    DirectionalLight(DirectionalLight&&) noexcept = default;

public:
    DirectionalLight& operator=(const DirectionalLight&) noexcept = default;
    DirectionalLight& operator=(DirectionalLight&&) noexcept = default;

    Eigen::Vector3f& direction() noexcept {
        return direction_;
    }

    const Eigen::Vector3f& direction() const noexcept {
        return direction_;
    }

    friend void to_json(nlohmann::json& json, const DirectionalLight& v);
    friend void from_json(const nlohmann::json& json, DirectionalLight& v);

private:
    Eigen::Vector3f direction_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_DIRECTIONALLIGHT_H
