//
//
#include <Volcano/Mesh.h>

VOLCANO_BEGIN

Mesh::Mesh(QObject *parent):
    Node(parent)
{
    setState(StateReady);
    setProgress(1.0);
}

Mesh::~Mesh(void)
{
}

void Mesh::setSource(const QUrl &v)
{
    if (m_source == v)
        return;

    m_source = v;

    setState(StateLoading);
    setProgress(0.0);

    // TODO start async loading...

    sourceChanged(v);
}

VOLCANO_END
