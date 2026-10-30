#include <cmath>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <string>

#include <nlohmann/json.hpp>
#include "Format.h"

#include <Volcano/Schema/Material.h>

VOLCANO_SCHEMA_BEGIN

void Material::validate() const {
    if (!albedo.allFinite() || (albedo.array() < 0).any() || (albedo.array() > 1).any() ||
        !std::isfinite(specular) || specular < 0 || specular > 1 ||
        !std::isfinite(metallic) || metallic < 0 || metallic > 1 ||
        !std::isfinite(roughness) || roughness < 0 || roughness > 1 ||
        !emissive.allFinite() || (emissive.array() < 0).any()) {
        throw std::invalid_argument("Material requires normalized albedo/coefficients and finite nonnegative emission");
    }
    if (alphaMode != AlphaMode::Opaque && alphaMode != AlphaMode::Blend) {
        throw std::invalid_argument("Unsupported material alpha mode");
    }

    detail::validatePath(albedoTexture, false);
    detail::validatePath(normalTexture, false);
    detail::validatePath(metallicRoughnessTexture, false);
}

Material Material::read(Stream& stream) {
    const auto bytes = stream.readAll();
    try {
        const auto document = detail::Json::parse(bytes.begin(), bytes.end());
        detail::requireJsonHeader(document, "material");
        const auto& albedo = document.at("albedo");
        const auto& emissive = document.at("emissive");
        if (!albedo.is_array() || albedo.size() != 4 || !emissive.is_array() || emissive.size() != 3) {
            throw std::invalid_argument("Material requires RGBA albedo and RGB emissive arrays");
        }

        Material result;
        for (int index = 0; index < 4; ++index) {
            result.albedo[index] = albedo.at(index).get<float>();
        }
        for (int index = 0; index < 3; ++index) {
            result.emissive[index] = emissive.at(index).get<float>();
        }
        result.specular = document.at("specular").get<float>();
        result.metallic = document.at("metallic").get<float>();
        result.roughness = document.at("roughness").get<float>();
        result.albedoTexture = detail::readPath(document.at("albedoTexture"));
        result.normalTexture = detail::readPath(document.at("normalTexture"));
        result.metallicRoughnessTexture = detail::readPath(document.at("metallicRoughnessTexture"));
        result.lit = document.at("lit").get<bool>();
        const auto mode = document.at("alphaMode").get<std::string>();
        if (mode == "opaque") {
            result.alphaMode = AlphaMode::Opaque;
        } else if (mode == "blend") {
            result.alphaMode = AlphaMode::Blend;
        } else {
            throw std::invalid_argument("Unsupported material alpha mode");
        }

        result.validate();
        return result;
    } catch (const detail::Json::exception& error) {
        throw std::invalid_argument(std::string("Invalid Material JSON: ") + error.what());
    }
}

void Material::write(Stream& stream) const {
    validate();
    const detail::Json document = {
        {"type", "material"}, {"version", 1},
        {"albedo", {albedo[0], albedo[1], albedo[2], albedo[3]}},
        {"specular", specular}, {"metallic", metallic}, {"roughness", roughness},
        {"emissive", {emissive[0], emissive[1], emissive[2]}},
        {"albedoTexture", detail::pathString(albedoTexture)},
        {"normalTexture", detail::pathString(normalTexture)},
        {"metallicRoughnessTexture", detail::pathString(metallicRoughnessTexture)},
        {"lit", lit}, {"alphaMode", alphaMode == AlphaMode::Opaque ? "opaque" : "blend"}
    };
    const auto text = document.dump(2) + '\n';
    stream.writeAll(std::span(reinterpret_cast<const std::uint8_t*>(text.data()), text.size()));
}

VOLCANO_SCHEMA_END
