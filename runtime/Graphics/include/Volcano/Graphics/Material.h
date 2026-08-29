#ifndef VOLCANO_GRAPHICS_MATERIAL_H
#define VOLCANO_GRAPHICS_MATERIAL_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Math.h>

VOLCANO_GRAPHICS_BEGIN

class Material final {
public:
    Material() noexcept;
    Material(const Eigen::Vector4f& albedo, float specular = 0.5f) noexcept;
    Material(const Material&) noexcept = default;
    Material(Material&&) noexcept = default;

public:
    Material& operator=(const Material&) noexcept = default;
    Material& operator=(Material&&) noexcept = default;

    Eigen::Vector4f& albedo() noexcept {
        return albedo_;
    }

    const Eigen::Vector4f& albedo() const noexcept {
        return albedo_;
    }

    float& specular() noexcept {
        return specular_;
    }

    const float& specular() const noexcept {
        return specular_;
    }

private:
    Eigen::Vector4f albedo_;
    float specular_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MATERIAL_H
