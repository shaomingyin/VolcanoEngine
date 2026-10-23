//
//
#include <Volcano/World/Material.h>

VOLCANO_WORLD_BEGIN

Material::Material() noexcept
    : Material(Volcano::Math::Vector4f(1.0f, 1.0f, 1.0f, 1.0f)) {
}

Material::Material(const Volcano::Math::Vector4f& albedo, float specular) noexcept
    : albedo_(albedo)
    , specular_(specular)
    , metallic_(0.0f)
    , roughness_(1.0f)
    , emissive_(0.0f, 0.0f, 0.0f) {
}

Volcano::Math::Vector4f& Material::albedo() noexcept {
    return albedo_;
}

const Volcano::Math::Vector4f& Material::albedo() const noexcept {
    return albedo_;
}

float& Material::specular() noexcept {
    return specular_;
}

const float& Material::specular() const noexcept {
    return specular_;
}

float& Material::metallic() noexcept {
    return metallic_;
}

const float& Material::metallic() const noexcept {
    return metallic_;
}

float& Material::roughness() noexcept {
    return roughness_;
}

const float& Material::roughness() const noexcept {
    return roughness_;
}

Volcano::Math::Vector3f& Material::emissive() noexcept {
    return emissive_;
}

const Volcano::Math::Vector3f& Material::emissive() const noexcept {
    return emissive_;
}

std::filesystem::path& Material::source() noexcept {
    return source_;
}

const std::filesystem::path& Material::source() const noexcept {
    return source_;
}

std::filesystem::path& Material::albedoTexture() noexcept {
    return albedo_texture_;
}

const std::filesystem::path& Material::albedoTexture() const noexcept {
    return albedo_texture_;
}

std::filesystem::path& Material::normalTexture() noexcept {
    return normal_texture_;
}

const std::filesystem::path& Material::normalTexture() const noexcept {
    return normal_texture_;
}

std::filesystem::path& Material::metallicRoughnessTexture() noexcept {
    return metallic_roughness_texture_;
}

const std::filesystem::path& Material::metallicRoughnessTexture() const noexcept {
    return metallic_roughness_texture_;
}

VOLCANO_WORLD_END
