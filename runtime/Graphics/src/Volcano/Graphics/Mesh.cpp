#include <Volcano/Graphics/Mesh.h>

#include <cstddef>

VOLCANO_GRAPHICS_BEGIN

Mesh::Mesh() {
}

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<index_type>&& indices)
    : vertex_buffer_(static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data())
    , index_buffer_(static_cast<GLsizeiptr>(indices.size() * sizeof(index_type)), indices.data())
    , count_(static_cast<GLsizei>(indices.size())) {
    vertex_array_.vertexAttribPointer(vertex_buffer_, 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position));
    vertex_array_.vertexAttribPointer(vertex_buffer_, 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, normal));
    vertex_array_.vertexAttribPointer(vertex_buffer_, 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, texcoord));

    vertex_array_.bind();
    index_buffer_.bind();
    gl::VertexArray::unbind();
}

VOLCANO_GRAPHICS_END
