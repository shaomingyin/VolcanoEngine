//
//
#ifndef VOLCANO_WORLD_MESH_H
#define VOLCANO_WORLD_MESH_H

#include <cstdint>
#include <filesystem>
#include <vector>

#include <Volcano/World/Bounds.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Mesh final {
public:
    struct Vertex final {
        Eigen::Vector3f position = { 0.0f, 0.0f, 0.0f };
        Eigen::Vector3f normal = { 0.0f, 1.0f, 0.0f };
        Eigen::Vector2f texcoord = { 0.0f, 0.0f };
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

    const Bounds& bounds() const noexcept;
    bool empty() const noexcept;

    std::filesystem::path& source() noexcept;
    const std::filesystem::path& source() const noexcept;

    void recalculateBounds() noexcept;

private:
    std::vector<Vertex> vertices_;
    std::vector<uint32_t> indices_;
    Bounds bounds_;
    std::filesystem::path source_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_MESH_H
