//
//
#include <Volcano/Mesh.hpp>

VOLCANO_BEGIN

Mesh::Mesh(QObject *parent):
    Component(parent)
{
}

Mesh::~Mesh(void)
{
}

const QUrl &Mesh::source(void) const
{
    return m_source;
}

void Mesh::setSource(const QUrl &url)
{
    if (m_source != url) {
        m_source = url;
        emit sourceChanged(url);
    }
}

VOLCANO_END
