//
//
#ifndef VOLCANO_OPENGLMESH_HPP
#define VOLCANO_OPENGLMESH_HPP

#include <Volcano/Common.hpp>
#include <Volcano/OpenGLMemory.hpp>

VOLCANO_BEGIN

class OpenGLMesh
{
public:
    OpenGLMesh(OpenGLMemory &memory);
    virtual ~OpenGLMesh(void);

public:
    bool init(int vertexCount, int vertexIndexCount);

private:
    OpenGLMemory &m_memory;
    OpenGLBuffer *m_vertexBuffer;
    OpenGLBuffer *m_vertexIndexBuffer;
};

VOLCANO_END

#endif // VOLCANO_OPENGLMESH_HPP
