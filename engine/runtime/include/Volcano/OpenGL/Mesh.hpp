//
//
#ifndef VOLCANO_OPENGL_MESH_HPP
#define VOLCANO_OPENGL_MESH_HPP

#include <Volcano/OpenGL/Common.hpp>

VOLCANO_OPENGL_BEGIN

class Mesh {
public:
    Mesh(void);
    virtual ~Mesh(void);

public:
    bool init(int vertexCount, int vertexIndexCount, bool isDynamic = false);

private:
    QIODevice *m_vertexBuffer;
    QIODevice *m_vertexIndexBuffer;
};

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_MESH_HPP
