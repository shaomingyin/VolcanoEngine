//
//
#ifndef VOLCANO_WORLD_MESH_H
#define VOLCANO_WORLD_MESH_H

#include <cstdint>
#include <filesystem>
#include <vector>

#include <Volcano/World/BoundingBox.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Mesh final {
public:
    struct Vertex final {
        Volcano::Math::Vector3f position = { 0.0f, 0.0f, 0.0f };
        Volcano::Math::Vector3f normal = { 0.0f, 1.0f, 0.0f };
        Volcano::Math::Vector2f texcoord = { 0.0f, 0.0f };
    };

public:
    Mesh() noexcept;
    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
    Mesh(const Mesh&) = default;

    Mesh(Mesh&&) noexcept = default;

public:
    Mesh& operator=(const Mesh&) = default;

    Mesh& operator=(Mesh&&) noexcept = default;

    const std::vector<Vertex>& vertices() const noexcept;
    std::vector<Vertex>& vertices() noexcept;

    const std::vector<uint32_t>& indices() const noexcept;
    std::vector<uint32_t>& indices() noexcept;

    const BoundingBox& bounds() const noexcept;
    bool empty() const noexcept;

    std::filesystem::path& source() noexcept;
    const std::filesystem::path& source() const noexcept;

    void recalculateBounds();

private:
    std::vector<Vertex> vertices_;
    std::vector<uint32_t> indices_;
    BoundingBox bounds_;
    std::filesystem::path source_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_MESH_H
