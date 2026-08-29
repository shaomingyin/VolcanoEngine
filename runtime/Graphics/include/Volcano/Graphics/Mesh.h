#ifndef VOLCANO_GRAPHICS_MESH_H
#define VOLCANO_GRAPHICS_MESH_H

#include <array>
#include <cstdint>
#include <vector>

#include <GL/gl.hpp>

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Mesh final {
public:
    struct Vertex final {
        std::array<float, 3> position = { 0.0f, 0.0f, 0.0f };
        std::array<float, 3> normal = { 0.0f, 0.0f, 1.0f };
        std::array<float, 2> texcoord = { 0.0f, 0.0f };
    };

    using index_type = uint32_t;

public:
    Mesh();
    Mesh(std::vector<Vertex>&& vertices, std::vector<index_type>&& indices);
    ~Mesh() = default;

    Mesh(const Mesh&) = delete;
    Mesh(Mesh&&) noexcept = default;

public:
    Mesh& operator=(const Mesh&) = delete;
    Mesh& operator=(Mesh&&) noexcept = default;

    bool empty() const noexcept {
        return count_ == 0;
    }

    const gl::VertexArray& vertexArray() const noexcept {
        return vertex_array_;
    }

    GLsizei count() const noexcept {
        return count_;
    }

private:
    gl::VertexArray vertex_array_;
    gl::VertexBuffer vertex_buffer_;
    gl::IndexBuffer index_buffer_;
    GLsizei count_ = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MESH_H
