#include <cmath>
#include <cstdint>
#include <limits>
#include <span>
#include <stdexcept>

#include <Volcano/Schema/Mesh.h>

#include "Format.h"

VOLCANO_SCHEMA_BEGIN

void Mesh::validate() const {
    if (vertices.size() > std::numeric_limits<std::uint32_t>::max() ||
        indices.size() > std::numeric_limits<std::uint32_t>::max() || indices.size() % 3 != 0) {
        throw std::invalid_argument("Mesh requires uint32 counts and triangle indices");
    }

    for (const auto& vertex : vertices) {
        for (const auto& attribute : {std::span<const float>(vertex.position),
            std::span<const float>(vertex.normal), std::span<const float>(vertex.texcoord)}) {
            for (const auto value : attribute) {
                if (!std::isfinite(value)) {
                    throw std::invalid_argument("Mesh vertex attributes must be finite");
                }
            }
        }
    }
    for (const auto index : indices) {
        if (index >= vertices.size()) {
            throw std::invalid_argument("Mesh index exceeds vertex count");
        }
    }
}

Mesh Mesh::read(Stream& stream) {
    const auto bytes = stream.readAll();
    std::span<const std::uint8_t> input(bytes);
    detail::requireBinaryHeader(input, "VEMH");
    const auto vertexCount = detail::readU32(input);
    const auto indexCount = detail::readU32(input);
    const auto payloadSize = detail::checkedSum(detail::checkedProduct(vertexCount, 32),
        detail::checkedProduct(indexCount, 4));
    if (input.size() != payloadSize || indexCount % 3 != 0) {
        throw std::invalid_argument("Mesh payload length or triangle index count is invalid");
    }

    Mesh result;
    result.vertices.resize(vertexCount);
    result.indices.resize(indexCount);
    for (auto& vertex : result.vertices) {
        for (const auto& attribute : {std::span<float>(vertex.position),
            std::span<float>(vertex.normal), std::span<float>(vertex.texcoord)}) {
            for (auto& value : attribute) {
                value = detail::readFloat(input);
            }
        }
    }
    for (auto& index : result.indices) {
        index = detail::readU32(input);
    }
    result.validate();
    return result;
}

void Mesh::write(Stream& stream) const {
    validate();
    ByteArray bytes{'V', 'E', 'M', 'H'};
    const auto payloadSize = detail::checkedSum(detail::checkedProduct(vertices.size(), 32),
        detail::checkedProduct(indices.size(), 4));
    bytes.reserve(detail::checkedSum(16, payloadSize));
    detail::appendU32(bytes, 1);
    detail::appendU32(bytes, static_cast<std::uint32_t>(vertices.size()));
    detail::appendU32(bytes, static_cast<std::uint32_t>(indices.size()));
    for (const auto& vertex : vertices) {
        for (const auto& attribute : {std::span<const float>(vertex.position),
            std::span<const float>(vertex.normal), std::span<const float>(vertex.texcoord)}) {
            for (const auto value : attribute) {
                detail::appendFloat(bytes, value);
            }
        }
    }
    for (const auto index : indices) {
        detail::appendU32(bytes, index);
    }

    stream.writeAll(bytes);
}

VOLCANO_SCHEMA_END
