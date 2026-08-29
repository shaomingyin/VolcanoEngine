//
//
#include <utility>

#include <Volcano/World/Mesh.h>

VOLCANO_WORLD_BEGIN

Mesh::Mesh() noexcept {
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
    : vertices_(std::move(vertices))
    , indices_(std::move(indices)) {
    recalculateBounds();
}

const std::vector<Mesh::Vertex>& Mesh::vertices() const noexcept {
    return vertices_;
}

std::vector<Mesh::Vertex>& Mesh::vertices() noexcept {
    return vertices_;
}

const std::vector<uint32_t>& Mesh::indices() const noexcept {
    return indices_;
}

std::vector<uint32_t>& Mesh::indices() noexcept {
    return indices_;
}

const Bounds& Mesh::bounds() const noexcept {
    return bounds_;
}

bool Mesh::empty() const noexcept {
    return vertices_.empty() || indices_.empty();
}

std::filesystem::path& Mesh::source() noexcept {
    return source_;
}

const std::filesystem::path& Mesh::source() const noexcept {
    return source_;
}

void Mesh::recalculateBounds() noexcept {
    bounds_.reset();
    for (const auto& vertex : vertices_) {
        bounds_.extend(vertex.position);
    }
}

VOLCANO_WORLD_END
