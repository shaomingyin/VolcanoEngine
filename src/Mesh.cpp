//
//
#include <Volcano/Mesh.hpp>

VOLCANO_BEGIN

Mesh::Mesh(Node *parent):
    Node(parent)
{
}

Mesh::~Mesh(void)
{
}

void Mesh::setSource(const QUrl &v)
{
    if (m_source == v)
        return;

    m_source = v;
    sourceChanged();


}

VOLCANO_END
