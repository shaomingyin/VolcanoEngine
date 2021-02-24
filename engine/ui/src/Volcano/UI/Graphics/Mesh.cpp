//
//
#include <Volcano/UI/Graphics/Mesh.hpp>

VOLCANO_UI_GRAPHICS_BEGIN

Mesh::Mesh(Memory &memory, QObject *parent):
    QObject(parent),
    m_memory(memory)
{
}

Mesh::~Mesh(void)
{
}

VOLCANO_UI_GRAPHICS_END
