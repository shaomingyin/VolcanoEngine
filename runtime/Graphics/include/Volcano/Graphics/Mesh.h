#ifndef VOLCANO_GRAPHICS_MESH_H
#define VOLCANO_GRAPHICS_MESH_H

#include <array>
#include <cstdint>
#include <span>
#include <string>

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
    Mesh(std::span<const Vertex> vertices, std::span<const index_type> indices);
    ~Mesh() = default;

    Mesh(const Mesh&) = delete;
    Mesh(Mesh&&) = delete;

public:
    Mesh& operator=(const Mesh&) = delete;
    Mesh& operator=(Mesh&&) = delete;

    bool empty() const noexcept {
        return count_ == 0;
    }

    void draw() const noexcept;
    static std::string shaderDefinitions();

public:
    static constexpr unsigned int PositionAttribute = 0;
    static constexpr unsigned int NormalAttribute = 1;
    static constexpr unsigned int TexcoordAttribute = 2;

private:
    gl::VertexArray vertex_array_;
    gl::VertexBuffer vertex_buffer_;
    gl::IndexBuffer index_buffer_;
    std::size_t count_ = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MESH_H
