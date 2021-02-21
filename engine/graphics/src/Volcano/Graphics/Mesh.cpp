//
//
#include <Volcano/Graphics/Mesh.hpp>

VOLCANO_GRAPHICS_BEGIN

Mesh::Mesh(Memory &memory, QObject *parent):
    QObject(parent),
    m_memory(memory)
{
}

Mesh::~Mesh(void)
{
}

VOLCANO_GRAPHICS_END
