//
//
#include <Volcano/Mesh.hpp>

VOLCANO_BEGIN

Mesh::Mesh(Node *parent):
    OpenGLResource(parent)
{
}

Mesh::~Mesh(void)
{
}

bool Mesh::initGL(void)
{
    Q_ASSERT(state() == StateFree || state() == StateDirty);



    return true;
}

VOLCANO_END
