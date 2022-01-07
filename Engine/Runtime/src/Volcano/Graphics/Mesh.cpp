//
//
#include <Volcano/Graphics/Mesh.hpp>

VOLCANO_GRAPHICS_BEGIN

Mesh::Mesh(QObject *parent):
    Shape(parent)
{
}

Mesh::~Mesh(void)
{
}

QUrl Mesh::source(void) const
{
    return m_source;
}

void Mesh::setSource(QUrl v)
{
    if (m_source != v) {
        m_source = v;
        emit sourceChanged(v);
    }
}

VOLCANO_GRAPHICS_END
