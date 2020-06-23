//
//
#include <Volcano/Renderer/Mesh.h>

VOLCANO_RENDERER_BEGIN

Mesh::Mesh(QObject *parent):
    Object(parent)
{
}

Mesh::~Mesh(void)
{
}

void Mesh::setSource(const QUrl &v)
{
    qDebug() << "source" << v;
    if (m_source == v)
        return;

    m_source = v;

    // TODO start async loading...

    sourceChanged(v);
}

VOLCANO_RENDERER_END
