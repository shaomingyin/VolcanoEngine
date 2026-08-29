#include <Volcano/Graphics/Material.h>

VOLCANO_GRAPHICS_BEGIN

Material::Material() noexcept
    : Material(Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f)) {
}

Material::Material(const Eigen::Vector4f& albedo, float specular) noexcept
    : albedo_(albedo)
    , specular_(specular) {
}

VOLCANO_GRAPHICS_END
