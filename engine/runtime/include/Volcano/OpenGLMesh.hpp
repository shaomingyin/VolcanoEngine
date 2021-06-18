//
//
#ifndef VOLCANO_OPENGLMESH_HPP
#define VOLCANO_OPENGLMESH_HPP

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class OpenGLMesh {
public:
    OpenGLMesh(void);
    virtual ~OpenGLMesh(void);

public:
    bool init(int vertexCount, int vertexIndexCount, bool isDynamic = false);

private:
    QIODevice *m_vertexBuffer;
    QIODevice *m_vertexIndexBuffer;
};

VOLCANO_END

#endif // VOLCANO_OPENGLMESH_HPP
