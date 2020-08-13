//
//
#include <QRunnable>

#include <Volcano/Mesh.hpp>

VOLCANO_BEGIN

Mesh::Mesh(Node *parent):
    Resource(parent)
{
}

Mesh::~Mesh(void)
{
}

void Mesh::onSourceChanged(const QUrl &url)
{
    if (m_vertexBuffer != nullptr || m_vertexIndexBuffer != nullptr)
        destroyBuffers();


}

void Mesh::createBuffers(void)
{

}

void Mesh::destroyBuffers(void)
{

}

VOLCANO_END
