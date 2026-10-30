#include <cstddef>
#include <limits>
#include <stdexcept>

#include <GL/gl.hpp>

#include <Volcano/Graphics/Mesh.h>

VOLCANO_GRAPHICS_BEGIN

std::string Mesh::shaderDefinitions() {
    return "#define POSITION_ATTRIBUTE " + std::to_string(PositionAttribute) + "\n"
        + "#define NORMAL_ATTRIBUTE " + std::to_string(NormalAttribute) + "\n"
        + "#define TEXCOORD_ATTRIBUTE " + std::to_string(TexcoordAttribute) + "\n";
}

Mesh::Mesh(std::span<const Vertex> vertices, std::span<const index_type> indices) {
    if (indices.size() > static_cast<std::size_t>(std::numeric_limits<GLsizei>::max()) || indices.size() % 3 != 0 ||
        vertices.size_bytes() > static_cast<std::size_t>(std::numeric_limits<GLsizeiptr>::max()) ||
        indices.size_bytes() > static_cast<std::size_t>(std::numeric_limits<GLsizeiptr>::max())) {
        throw std::invalid_argument("Mesh requires triangle indices and buffers that fit OpenGL limits");
    }

    for (auto index : indices) {
        if (index >= vertices.size()) {
            throw std::out_of_range("Mesh index exceeds vertex count");
        }
    }

    vertex_array_.bind();
    vertex_buffer_.data(static_cast<GLsizeiptr>(vertices.size_bytes()), vertices.data());
    // A core-profile element buffer belongs to the currently bound VAO.
    index_buffer_.data(static_cast<GLsizeiptr>(indices.size_bytes()), indices.data());
    vertex_array_.vertexAttribPointer(vertex_buffer_, Mesh::PositionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position));
    vertex_array_.vertexAttribPointer(vertex_buffer_, Mesh::NormalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, normal));
    vertex_array_.vertexAttribPointer(vertex_buffer_, Mesh::TexcoordAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, texcoord));
    count_ = static_cast<GLsizei>(indices.size());
    gl::VertexArray::unbind();
}

void Mesh::draw() const noexcept {
    if (empty()) {
        return;
    }

    vertex_array_.bind();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count_), GL_UNSIGNED_INT, nullptr);
    gl::VertexArray::unbind();
}

VOLCANO_GRAPHICS_END
