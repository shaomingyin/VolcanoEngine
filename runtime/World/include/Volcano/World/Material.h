//
//
#ifndef VOLCANO_WORLD_MATERIAL_H
#define VOLCANO_WORLD_MATERIAL_H

#include <filesystem>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Material final {
public:
    Material() noexcept;
    Material(const Volcano::Math::Vector4f& albedo, float specular = 0.5f) noexcept;
    Material(const Material&) = default;
    Material(Material&&) noexcept = default;

public:
    Material& operator=(const Material&) = default;
    Material& operator=(Material&&) noexcept = default;

    Volcano::Math::Vector4f& albedo() noexcept;
    const Volcano::Math::Vector4f& albedo() const noexcept;

    float& specular() noexcept;
    const float& specular() const noexcept;

    float& metallic() noexcept;
    const float& metallic() const noexcept;

    float& roughness() noexcept;
    const float& roughness() const noexcept;

    Volcano::Math::Vector3f& emissive() noexcept;
    const Volcano::Math::Vector3f& emissive() const noexcept;

    std::filesystem::path& source() noexcept;
    const std::filesystem::path& source() const noexcept;

    std::filesystem::path& albedoTexture() noexcept;
    const std::filesystem::path& albedoTexture() const noexcept;

    std::filesystem::path& normalTexture() noexcept;
    const std::filesystem::path& normalTexture() const noexcept;

    std::filesystem::path& metallicRoughnessTexture() noexcept;
    const std::filesystem::path& metallicRoughnessTexture() const noexcept;

private:
    Volcano::Math::Vector4f albedo_;
    float specular_;
    float metallic_;
    float roughness_;
    Volcano::Math::Vector3f emissive_;
    std::filesystem::path source_;
    std::filesystem::path albedo_texture_;
    std::filesystem::path normal_texture_;
    std::filesystem::path metallic_roughness_texture_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_MATERIAL_H
