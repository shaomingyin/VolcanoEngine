//
//
#include <Volcano/Game/Mesh.hpp>

VOLCANO_GAME_BEGIN

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

VOLCANO_GAME_END
