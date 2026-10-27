#ifndef VOLCANO_SCHEMA_MATERIAL_H
#define VOLCANO_SCHEMA_MATERIAL_H

#include <filesystem>

#include <Eigen/Core>

#include <Volcano/Schema/Common.h>
#include <Volcano/Stream.h>

VOLCANO_SCHEMA_BEGIN

struct Material final {
public:
    enum class AlphaMode {
        Opaque,
        Blend
    };

    void validate() const;
    static Material read(Stream& stream);
    void write(Stream& stream) const;

public:
    Eigen::Vector4f albedo = Eigen::Vector4f::Ones();
    float specular = 0.5f;
    float metallic = 0.0f;
    float roughness = 1.0f;
    Eigen::Vector3f emissive = Eigen::Vector3f::Zero();
    std::filesystem::path albedoTexture;
    std::filesystem::path normalTexture;
    std::filesystem::path metallicRoughnessTexture;
    bool lit = true;
    AlphaMode alphaMode = AlphaMode::Opaque;
};

VOLCANO_SCHEMA_END

#endif // VOLCANO_SCHEMA_MATERIAL_H
