//
//
#ifndef VOLCANO_MESH_HPP
#define VOLCANO_MESH_HPP

#include <QIODevice>

#include <Volcano/Common.hpp>
#include <Volcano/OpenGLResource.hpp>

VOLCANO_BEGIN

class VOLCANO_API Mesh: public OpenGLResource
{
    Q_OBJECT

public:
    Mesh(Node *parent = nullptr);
    ~Mesh(void) override;

public:
    bool initGL(void) override;

private:
    QIODevice *m_vertexBuffer;
    QIODevice *m_vertexIndexBuffer;
};

VOLCANO_END

#endif // VOLCANO_MESH_HPP
